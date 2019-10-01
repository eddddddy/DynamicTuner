#include <cmath>
#include <iostream>
#include <list>
#include <set>
#include <algorithm>

#include "frac.h"
#include "pitch.h"
#include "tunings.h"


EPitchFreq NoteTuning::getEPitchFreq(double relFreq) const {
    return EPitchFreq{pitch, tuning * relFreq};
}

bool operator<(const NoteTuning& nt1, const NoteTuning& nt2) {
    return (nt1.pitch < nt2.pitch) ? true : (nt1.pitch == nt2.pitch && nt1.tuning < nt2.tuning);
}

bool operator==(const NoteTuning& nt1, const NoteTuning& nt2) {
    return (nt1.pitch == nt2.pitch) && (nt1.tuning == nt2.tuning);
}

bool operator==(const NoteTuning& nt, const EPitch& p) {
    return nt.pitch == p;
}

bool operator==(const EPitch& p, const NoteTuning& nt) {
    return nt.pitch == p;
}

Tuning::TuningIter::TuningIter(std::multiset<NoteTuning>::iterator it): it{it} {}

Tuning::TuningIter& Tuning::TuningIter::operator++() {
    ++it;
    return *this;
}

NoteTuning Tuning::TuningIter::operator*() const {
    return *it;
}

bool Tuning::TuningIter::operator!=(const Tuning::TuningIter& other) const {
    return it != other.it;
}

Tuning::Tuning(): noteTunings{std::multiset<NoteTuning>{}} {}

Tuning& Tuning::addNoteTuning(const NoteTuning& nt) {
    noteTunings.insert(nt);
    return *this;
}

bool Tuning::removeNoteTuning(const NoteTuning& nt) {
    auto it = noteTunings.find(nt);
    if (it != noteTunings.end()) {
        noteTunings.erase(it);
        return true;
    }
    return false;
}

NoteTuning Tuning::removePitch(const EPitch& ep) {
    auto it = std::find(noteTunings.begin(), noteTunings.end(), ep);
    if (it != noteTunings.end()) {
		NoteTuning nt = *it;
        noteTunings.erase(it);
        return nt;
    }
    return NoteTuning{EPitch{}, Frac{0, 1}};
}

bool Tuning::isEmpty() const {
    return noteTunings.empty();
}

Tuning Tuning::split(const std::list<EPitch>& filter) {
    Tuning other;
    for (const EPitch& pitch : filter) {
        auto it = std::find(noteTunings.begin(), noteTunings.end(), pitch);
        noteTunings.erase(it);
        other.addNoteTuning(*it);
    }
    return other;
}

std::vector<EPitchFreq> Tuning::getEPitchFreqs(double relFreq) const {
    std::vector<EPitchFreq> v;
    for (const NoteTuning& nt : noteTunings) {
        v.emplace_back(nt.getEPitchFreq(relFreq));
    }
    return v;
}

Tuning Tuning::operator+(const NoteTuning& nt) const {
    Tuning newTuning{*this};
    newTuning.addNoteTuning(nt);
    return newTuning;
}

TuningSequence Tuning::operator+(const TuningSequence& ts) const {
    TuningSequence newTuningSequence{ts};
    newTuningSequence.addTuningFront(*this);
    return newTuningSequence;
}

bool Tuning::operator<(const Tuning& other) const {
    return noteTunings < other.noteTunings;
}

Tuning::TuningIter Tuning::begin() const {
    return TuningIter(noteTunings.begin());
}

Tuning::TuningIter Tuning::end() const {
    return TuningIter{noteTunings.end()};
}

TuningSequence::TuningSequenceIter::TuningSequenceIter(std::list<Tuning>::const_iterator it): it{it} {}

TuningSequence::TuningSequenceIter& TuningSequence::TuningSequenceIter::operator++() {
    ++it;
    return *this;
}
        
Tuning TuningSequence::TuningSequenceIter::operator*() const {
    return *it;
}

bool TuningSequence::TuningSequenceIter::operator!=(const TuningSequence::TuningSequenceIter& other) const {
    return it != other.it;
}

TuningSequence::TuningSequence(): tunings{std::list<Tuning>{}} {}

TuningSequence& TuningSequence::addTuningFront(const Tuning& t) {
    tunings.emplace_front(t);
    return *this;
}

TuningSequence& TuningSequence::addTuning(const Tuning& t) {
    tunings.emplace_back(t);
    return *this;
}

std::vector<std::vector<EPitchFreq>> TuningSequence::getFreqs() const {
    std::vector<std::vector<EPitchFreq>> v;
    if (tunings.empty()) return v;

    NoteTuning nt = *(*tunings.begin()).begin();
    double relFreq = getNormalFreq(nt.pitch) / nt.tuning;

    for (const Tuning& t : tunings) {
        v.emplace_back(t.getEPitchFreqs(relFreq));
    }
    return v;
}

bool TuningSequence::operator<(const TuningSequence& other) const {
    return tunings < other.tunings;
}

TuningSequence::TuningSequenceIter TuningSequence::begin() const {
    return TuningSequenceIter{tunings.begin()};
}

TuningSequence::TuningSequenceIter TuningSequence::end() const {
    return TuningSequenceIter{tunings.end()};
}

double getNormalFreq(const EPitch& pitch) {
    int offset = (static_cast<int>(pitch.pitch) - static_cast<int>(Pitch::A) + 12) % 12
               + 12 * (pitch.octave - 4 + (pitch.pitch < Pitch::A ? -1 : 0));
    return 440.0 * std::pow(2.0, offset / 12.0);
}

std::ostream& operator<<(std::ostream& out, const NoteTuning& nt) {
    out << "(" << nt.pitch << " " << nt.tuning << ")";
    return out;
}

std::ostream& operator<<(std::ostream& out, const Tuning& t) {
    for (NoteTuning nt : t) {
        out << " " << nt;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const TuningSequence& ts) {
    for (Tuning t : ts) {
        out << t << "\n";
    }
    return out;
}
