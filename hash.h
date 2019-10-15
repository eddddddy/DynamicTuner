#ifndef _HASH_H_
#define _HASH_H_

enum class Int;
struct EPitch;
struct NoteTuning;

struct Hash {
    std::size_t operator()(const EPitch& p) const;
    std::size_t operator()(const EPitchFreq& p) const;
    std::size_t operator()(const NoteTuning& nt) const;
};

#endif
