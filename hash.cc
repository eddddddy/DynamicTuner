#include <functional>

#include "pitch.h"
#include "interval.h"
#include "tunings.h"
#include "hash.h"

std::size_t Hash::operator()(const EPitch& p) const {
    return std::hash<int>()(12 * p.octave + static_cast<int>(p.pitch));
}

std::size_t Hash::operator()(const EPitchFreq& p) const {
    return operator()(p.pitch);
}

std::size_t Hash::operator()(const NoteTuning& nt) const {
    std::size_t seed = 0;
    seed ^= operator()(nt.pitch) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<long>()(nt.tuning.p) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<long>()(nt.tuning.q) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}
