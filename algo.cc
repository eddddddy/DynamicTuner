#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <iterator>
#include <algorithm>
#include <utility>

#include "frac.h"
#include "pitch.h"
#include "interval.h"
#include "tunings.h"
#include "algo.h"
#include "score.h"


std::map<Tuning, int> Algo::getValuesRec(const Tuning& fixed, std::list<EPitch> var) {

    if (var.empty()) {
        std::map<Tuning, int> m{};
        m[Tuning{}] = 0;
        return m;
    }

    if (fixed.isEmpty()) {
        EPitch pivot = *(var.begin());
        var.erase(var.begin());
        std::map<Tuning, int> m = getValuesRec(Tuning{}.addNoteTuning(NoteTuning{pivot, Frac{1, 1}}), var);
        std::map<Tuning, int> mNew{};
        for (std::pair<Tuning, int> pair : m) {
            Tuning tuning = pair.first;
            tuning.addNoteTuning(NoteTuning{pivot, Frac{1, 1}});
            mNew[tuning] = pair.second;
        }
        return mNew;
    }

    std::map<Tuning, int> m{};
    for (EPitch pitch : var) {
        Tuning fixedCopy{fixed};
        while (!fixedCopy.isEmpty()) {
            NoteTuning relNoteTuning = *(fixedCopy.begin());

            std::list<EPitch> varCopy{var};
            varCopy.erase(std::find(varCopy.begin(), varCopy.end(), pitch));

            EPitch relPitch = relNoteTuning.pitch;
            Frac computedRatio = Interval{relPitch, pitch}.getIdealRatio() * relNoteTuning.tuning;

            int octaveOffset = pitch.octave - relPitch.octave + (pitch.pitch < relPitch.pitch ? -1 : 0);
            computedRatio.adjust(octaveOffset);

            std::map<Tuning, int> mSub = getValuesRec(fixed + NoteTuning{pitch, computedRatio}, varCopy);

            int valueToAdd = 0;
            for (NoteTuning nt : fixed) {
                Frac ideal = Interval{nt.pitch, pitch}.getIdealRatio().adjust();
                if (ideal == (computedRatio / nt.tuning).adjust()) {
                    valueToAdd += Interval{nt.pitch, pitch}.getWeight();
                    fixedCopy.removeNoteTuning(nt);
                } else if (ideal == (nt.tuning / computedRatio).adjust()) {
                    valueToAdd += Interval{nt.pitch, pitch}.getWeight();
                }
            }

            for (std::pair<Tuning, int> pair : mSub) {
                Tuning tuning = pair.first;
                tuning.addNoteTuning(NoteTuning{pitch, computedRatio});
                m[tuning] = pair.second + valueToAdd;
            }
        }
    }

    return m;
}

std::multimap<int, Tuning> Algo::getValues(const Tuning& fixed, const std::list<EPitch>& var) {
    std::map<Tuning, int> m = getValuesRec(fixed, var);
    std::multimap<int, Tuning> mm{};
    for (auto pair : m) {
        mm.insert(std::pair<int, Tuning>{pair.second, pair.first});
    }
    return mm;
}

std::vector<Tuning> Algo::getBestValues(const Tuning& fixed, const std::list<EPitch>& var) {
    std::multimap<int, Tuning> mm = getValues(fixed, var);
    std::vector<Tuning> v;
    if (mm.empty()) return v;
    auto rit = mm.rbegin();
    int bestValue = (*rit).first;
    while ((rit != mm.rend()) && (*rit).first == bestValue) {
        v.emplace_back((*rit).second);
        ++rit;
    }
    return v;
}

std::vector<TuningSequence> Algo::getTuningsRec(std::multimap<int, Tuning> prev, std::list<std::list<EPitch>> next, int& value, unsigned int trim) {

    if (next.empty()) {
        std::vector<TuningSequence> v;
        auto rit = prev.rbegin();
        int bestValue = (*rit).first;
        while ((rit != prev.rend()) && (*rit).first == bestValue) {
            TuningSequence ts;
            ts.addTuning((*rit).second);
            v.emplace_back(ts);
            ++rit;
        }
        value = bestValue;
        return v;
    }

    if ((trim > 0) && (prev.size() > trim)) {
        std::multimap<int, Tuning> prevCopy;
        unsigned int i = 0;
        for (auto it = prev.rbegin(); i < trim; ++it) {
            prevCopy.insert(*it);
            i++;
        }
        prev = std::move(prevCopy);
    }

    std::map<Tuning, std::pair<std::set<Tuning>, int>> backMap;
    for (auto& prevTuning : prev) {
        std::multimap<int, Tuning> nextTunings = getValues(prevTuning.second, *(next.begin()));
        for (auto& nextTuning : nextTunings) {
            int val = nextTuning.first;
            Tuning tuning = nextTuning.second;
            try {
                if (prevTuning.first + val > backMap.at(tuning).second) {
                    backMap[tuning] = std::pair<std::set<Tuning>, int>{std::set<Tuning>{prevTuning.second}, prevTuning.first + val};
                } else if (prevTuning.first + val == backMap.at(tuning).second) {
                    backMap[tuning].first.insert(prevTuning.second);
                }
            } catch (std::out_of_range& e) {
                backMap[tuning] = std::pair<std::set<Tuning>, int>{std::set<Tuning>{prevTuning.second}, prevTuning.first + val};
            }
        }
    }

    std::multimap<int, Tuning> nextPrev;
    for (auto& pair : backMap) {
        nextPrev.insert(std::pair<int, Tuning>{pair.second.second, pair.first});
    }
    next.erase(next.begin());
    std::vector<TuningSequence> bestNextTunings = getTuningsRec(nextPrev, next, value, trim);

    std::vector<TuningSequence> v;
    for (TuningSequence& ts : bestNextTunings) {
        std::set<Tuning> backMapped = backMap[*(ts.begin())].first;
        for (auto& prevTuning : backMapped) {
            v.emplace_back(prevTuning + ts);
        }
    }

    return v;
}

std::vector<TuningSequence> Algo::getTunings(std::list<std::list<EPitch>> seq, int* val) {

    int s = seq.size();

    if (s == 0) return std::vector<TuningSequence>{};

    if (s == 1) {
        std::vector<Tuning> bestTunings = getBestValues(Tuning{}, *(seq.begin()));
        std::vector<TuningSequence> v;
        for (Tuning& tuning : bestTunings) {
            v.emplace_back(TuningSequence{}.addTuning(tuning));
        }
        return v;
    }

    if (s == 2) {
        std::vector<TuningSequence> v;
        std::list<EPitch> secondNotes{*std::next(seq.begin())};
        (*seq.begin()).splice((*seq.begin()).end(), *std::next(seq.begin()));
        std::vector<Tuning> bestTunings = getBestValues(Tuning{}, *seq.begin());
        for (Tuning& tuning : bestTunings) {
            Tuning second = tuning.split(secondNotes);
            v.emplace_back(TuningSequence{}.addTuning(tuning).addTuning(second));
        }
        return v;
    }

    std::list<EPitch> secondNotes{*std::next(seq.begin())};
    (*seq.begin()).splice((*seq.begin()).end(), *std::next(seq.begin()));

    std::multimap<int, Tuning> startTunings = getValues(Tuning{}, *seq.begin());
    std::vector<TuningSequence> v;
    int bestValue = -1;

    for (auto& pair : startTunings) {
        Tuning& first = pair.second;
        Tuning second = first.split(secondNotes);
        int value = -1;
        std::multimap<int, Tuning> next;
        next.insert(std::pair<int, Tuning>{pair.first, second});

        std::vector<TuningSequence> bestTunings = getTuningsRec(next, std::list<std::list<EPitch>>{std::next(seq.begin(), 2), seq.end()}, value, 16);

        if (value > bestValue) {
            bestValue = value;
            v.clear();
            for (TuningSequence& bestTuning : bestTunings) {
                v.emplace_back(first + bestTuning);
            }
        } else if (value == bestValue) {
            for (TuningSequence& bestTuning : bestTunings) {
                v.emplace_back(first + bestTuning);
            }
        }
    }

    if (val) *val = bestValue;
    return v;
}

