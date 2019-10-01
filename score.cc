#include <iostream>
#include <vector>
#include <map>

#include "pitch.h"
#include "tunings.h"
#include "score.h"
#include "algo.h"


Score::Score(): score{std::vector<std::map<EPitchFreq, bool>>{}}, notes{std::vector<std::list<EPitchFreq>>{}}, pitches{std::vector<std::list<EPitch>>{}} {}

int Score::getLength() const {
    return score.size();
}

Score& Score::add(const Note& note) {
    return add(note.pitch.pitch, note.duration, note.startBeat);
}

Score& Score::add(const EPitch& pitch, uint32_t duration, uint32_t time) {
    if (time + duration - 1 > score.size()) {
        score.resize(time + duration - 1);
        notes.resize(time + duration - 1);
        pitches.resize(time + duration - 1);
    }

    for (uint32_t t = time; t < time + duration; t++) {
        std::map<EPitchFreq, bool>& at = score[t - 1];
        auto find = at.find(EPitchFreq{pitch, 0.0});
        if (find != at.end()) {
            at.emplace_hint(find, EPitchFreq{pitch, 0.0}, (t == time) || find->second);
        } else {
            at[EPitchFreq{pitch, 0.0}] = (t == time);
            notes[t - 1].emplace_back(EPitchFreq{pitch, 0.0});
            pitches[t - 1].emplace_back(pitch);
        }
    }

    return *this;
}

void Score::calculateFreqs() {
    TuningSequence tuning = Algo::getTunings(std::list<std::list<EPitch>>{pitches.begin(), pitches.end()})[0];
    std::vector<std::vector<EPitchFreq>> freqs = tuning.getFreqs();

    // For now, assume the score has no breaks in it
    for (unsigned int i = 0; i < freqs.size(); i++) {
        std::map<EPitchFreq, bool>& mapAt = score[i];
        std::list<EPitchFreq> newNotesAt;

        for (EPitchFreq& ep : freqs[i]) {
            auto nodeHandler = mapAt.extract(ep);
            nodeHandler.key() = ep;
            mapAt.insert(std::move(nodeHandler));

            newNotesAt.emplace_back(ep);
        }

        notes[i] = newNotesAt;
    }
}

Score::BeatIter::BeatIter(std::vector<std::list<EPitchFreq>>::const_iterator it): it{it} {}

Score::BeatIter& Score::BeatIter::operator++() {
    ++it;
    return *this;
}

std::list<EPitchFreq> Score::BeatIter::operator*() const {
    return *it;
}

bool Score::BeatIter::operator!=(const Score::BeatIter& other) const {
    return it != other.it;
}

Score::BeatIter Score::begin() const {
    return Score::BeatIter{notes.begin()};
}

Score::BeatIter Score::end() const {
    return Score::BeatIter{notes.end()};
}

Score::NoteIter::NoteIter(std::vector<std::map<EPitchFreq, bool>>::const_iterator begin, std::vector<std::map<EPitchFreq, bool>>::const_iterator end): begin{begin}, end{end}, beat{1} {
    if (begin != end) {
        it = (*begin).begin();
    } else {
        it = (*begin).end();
    }
}

Score::NoteIter& Score::NoteIter::operator++() {
    do {
        ++it;
        if (it == (*begin).end()) {
            if (begin == end) return *this;
            ++begin;
            ++beat;
            it = (*begin).begin();
        }
    } while (!(*it).second);

    return *this;
}

Note Score::NoteIter::operator*() const {
    int duration = 0;
    auto beginCopy{begin};
    const EPitchFreq& pitch = it->first;
    while (beginCopy <= end) {
        auto find = (*beginCopy).find(pitch);
        if ((find != (*beginCopy).end()) && !find->second) {
            duration++;
            ++beginCopy;
        } else break;
    }
    return Note{pitch, duration, beat};
}

bool Score::NoteIter::operator!=(const Score::NoteIter& other) const {
    return (begin != other.begin) || (it != other.it);
}

Score::NoteIter Score::nbegin() const {
    return Score::NoteIter{score.begin(), score.end() - 1};
}

Score::NoteIter Score::nend() const {
    return Score::NoteIter{score.end() - 1, score.end() - 1};
}

