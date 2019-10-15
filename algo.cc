#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <utility>

#include "frac.h"
#include "pitch.h"
#include "interval.h"
#include "tunings.h"
#include "hash.h"
#include "algo.h"
#include "score.h"

// Helper function that returns true if dividend divided by divisor
//   is congruent to other. Congruent in this case means offset by
//   a factor of 2.
// We rely on the fact that we won't get any false positives if an
//   overflow occurs (we won't get any true negatives because the
//   lowest 64 bits would still be the same).
bool isQuotientCongruentTo(const Frac& dividend, const Frac& divisor, const Frac& other) {
    unsigned long a = dividend.p * divisor.q * other.q;
    unsigned long b = dividend.q * divisor.p * other.p;
    if (a == 0 || b == 0) return true;

    int actz = __builtin_ctzl(a);
    int bctz = __builtin_ctzl(b);
    if (actz > bctz) return a == (b << (actz - bctz));
    else return b == (a << (bctz - actz));
}

// Returns a list of pairs with first element in fixed and second element in var
//   representing the pairs to check when optimizing var tunings.
std::list<std::pair<NoteTuning, EPitch>> findPairsToCheck(const Tuning& fixed, const std::list<EPitch>& var) {

    static int unisonValue = Interval::getWeight(EPitch{Pitch::C, 4}, EPitch{Pitch::C, 4});

    std::unordered_map<EPitch, std::list<NoteTuning>, Hash> varToFixed;
    int currBestValue = -1;
    for (const NoteTuning& f : fixed) {
        for (const EPitch& v : var) {
            int val = Interval::getWeight(f.pitch, v);
            if (val > currBestValue) {
                currBestValue = val;
                varToFixed.clear();
                varToFixed[v].emplace_back(f);
            } else if (val == currBestValue) {
                varToFixed[v].emplace_back(f);
            }
        }
    }

    std::list<std::pair<NoteTuning, EPitch>> bestPairs;
    int currBestValue2 = -1;
    for (const EPitch& v : var) {
        for (const EPitch& w : var) {
            if (!(w == v)) {
                int val = Interval::getWeight(v, w);
                if (val > currBestValue2) {
                    currBestValue2 = val;
                    bestPairs.clear();
                    for (const NoteTuning& f : fixed) {
                        bestPairs.emplace_back(std::pair<NoteTuning, EPitch>{f, v});
                    }
                } else if (val == currBestValue2) {
                    for (const NoteTuning& f : fixed) {
                        bestPairs.emplace_back(std::pair<NoteTuning, EPitch>{f, v});
                    }
                }
            }
        }
    }

    if (currBestValue > currBestValue2 || currBestValue == unisonValue) {
        bestPairs.clear();
        auto varToFixedList = *varToFixed.begin();
        for (const NoteTuning& nt : varToFixedList.second) {
            bestPairs.emplace_back(std::pair<NoteTuning, EPitch>(nt, varToFixedList.first));
        }
    }

    return bestPairs;
}

std::map<Tuning, int> Algo::getValuesRec(const Tuning& fixed, std::list<EPitch> var,
    std::unordered_map<EPitch, std::unordered_set<NoteTuning, Hash>, Hash> calculated) {

    if (var.empty()) {
        std::map<Tuning, int> m{};
        m[Tuning{}] = 0;
        return m;
    }

    if (fixed.isEmpty()) {
        EPitch pivot = *(var.begin());
        var.erase(var.begin());
        std::map<Tuning, int> m = getValuesRec(Tuning{}.addNoteTuning(NoteTuning{pivot, Frac{1, 1}}), var, calculated);
        std::map<Tuning, int> mNew{};
        for (std::pair<Tuning, int> pair : m) {
            Tuning tuning = pair.first;
            tuning.addNoteTuning(NoteTuning{pivot, Frac{1, 1}});
            mNew[tuning] = pair.second;
        }
        return mNew;
    }

    std::map<Tuning, int> m{};
    std::list<std::pair<NoteTuning, EPitch>> fixedVarPairs = findPairsToCheck(fixed, var);
    while (!fixedVarPairs.empty()) {

        std::pair<NoteTuning, EPitch> pair = *fixedVarPairs.begin();
        EPitch pitch = pair.second;
        EPitch relPitch = pair.first.pitch;

        std::list<EPitch> varCopy{var};
        varCopy.erase(std::find(varCopy.begin(), varCopy.end(), pitch));

        Frac computedRatio = Interval::getIdealRatio(relPitch, pitch) * pair.first.tuning;
        int octaveOffset = pitch.octave - relPitch.octave + (pitch.pitch < relPitch.pitch ? -1 : 0);
        computedRatio.adjust(octaveOffset);

        calculated[pitch].emplace(pair.first);
        std::map<Tuning, int> mSub = getValuesRec(fixed + NoteTuning{pitch, computedRatio}, varCopy, calculated);

        int valueToAdd = 0;
        for (NoteTuning nt :fixed) {
            Frac ideal = Interval::getIdealRatio(nt.pitch, pitch);
            if (isQuotientCongruentTo(computedRatio, nt.tuning, ideal)) {
                valueToAdd += Interval::getWeight(nt.pitch, pitch);
                fixedVarPairs.remove(std::pair<NoteTuning, EPitch>{nt, pitch});
            } else if (isQuotientCongruentTo(nt.tuning, computedRatio, ideal)) {
                valueToAdd += Interval::getWeight(nt.pitch, pitch);
            }
        }

        for (std::pair<Tuning, int> pair : mSub) {
            Tuning tuning = pair.first;
            tuning.addNoteTuning(NoteTuning{pitch, computedRatio});
            m[tuning] = pair.second + valueToAdd;
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

        std::vector<TuningSequence> bestTunings = getTuningsRec(next, std::list<std::list<EPitch>>{std::next(seq.begin(), 2), seq.end()}, value, 8);

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
