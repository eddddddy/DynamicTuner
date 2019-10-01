#include <iostream>

#include "pitch.h"

bool operator<(Pitch p1, Pitch p2) {
    return static_cast<int>(p1) < static_cast<int>(p2);
}

bool operator<=(Pitch p1, Pitch p2) {
    return static_cast<int>(p1) <= static_cast<int>(p2);
}

bool operator==(Pitch p1, Pitch p2) {
    return static_cast<int>(p1) == static_cast<int>(p2);
}

bool operator>(Pitch p1, Pitch p2) {
    return static_cast<int>(p1) > static_cast<int>(p2);
}

bool operator>=(Pitch p1, Pitch p2) {
    return static_cast<int>(p1) >= static_cast<int>(p2);
}

std::ostream& operator<<(std::ostream& out, Pitch pitch) {
    switch (pitch) {
        case Pitch::C:  out << "C";  return out;
        case Pitch::Cs: out << "Cs"; return out;
        case Pitch::D:  out << "D";  return out;
        case Pitch::Ds: out << "Ds"; return out;
        case Pitch::E:  out << "E";  return out;
        case Pitch::F:  out << "F";  return out;
        case Pitch::Fs: out << "Fs"; return out;
        case Pitch::G:  out << "G";  return out;
        case Pitch::Gs: out << "Gs"; return out;
        case Pitch::A:  out << "A";  return out;
        case Pitch::As: out << "As"; return out;
        case Pitch::B:  out << "B";  return out;
        default:                     return out;
    }
}

bool operator<(const EPitch& ep1, const EPitch& ep2) {
    if (ep1.octave < ep2.octave) {
        return true;
    } else if ((ep1.octave == ep2.octave) && (ep1.pitch < ep2.pitch)) {
        return true;
    }
    return false;
}

bool operator<=(const EPitch& ep1, const EPitch& ep2) {
    return (ep1 < ep2) || (ep1 == ep2);
}

bool operator==(const EPitch& ep1, const EPitch& ep2) {
    return (ep1.pitch == ep2.pitch) && (ep1.octave == ep2.octave);
}

bool operator>(const EPitch& ep1, const EPitch& ep2) {
    return !(ep1 <= ep2);
}

bool operator>=(const EPitch& ep1, const EPitch& ep2) {
    return !(ep1 < ep2);
}

std::ostream& operator<<(std::ostream& out, const EPitch& ePitch) {
    out << ePitch.pitch << "-" << ePitch.octave;
    return out;
}

bool operator<(const EPitchFreq& p1, const EPitchFreq& p2) {
    return p1.pitch < p2.pitch;
}

bool operator<=(const EPitchFreq& p1, const EPitchFreq& p2) {
    return p1.pitch <= p2.pitch;
}

bool operator==(const EPitchFreq& p1, const EPitchFreq& p2) {
    return p1.pitch == p2.pitch;
}

bool operator>(const EPitchFreq& p1, const EPitchFreq& p2) {
    return p1.pitch > p2.pitch;
}

bool operator>=(const EPitchFreq& p1, const EPitchFreq& p2) {
    return p1.pitch >= p2.pitch;
}

std::ostream& operator<<(std::ostream& out, const EPitchFreq& f) {
    out << "(" << f.pitch << " " << f.freq << ")";
    return out;
}
