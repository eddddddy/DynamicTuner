#include "frac.h"
#include "pitch.h"
#include "interval.h"

Frac Interval::idealRatios[12] = {
    Frac{1, 1}  ,
    Frac{16, 15},
    Frac{9, 8}  ,
    Frac{6, 5}  ,
    Frac{5, 4}  ,
    Frac{4, 3}  ,
    Frac{45, 32},
    Frac{3, 2}  ,
    Frac{8, 5}  ,
    Frac{5, 3}  ,
    Frac{16, 9} ,
    Frac{15, 8} 
};

int Interval::weights[12] = {65536, 4, 8, 64, 64, 1024, 1, 1024, 64, 64, 8, 4};

Frac Interval::getIdealRatio(const EPitch& ep1, const EPitch& ep2) {
    return idealRatios[(static_cast<int>(ep2.pitch) - static_cast<int>(ep1.pitch) + 12) % 12];
}

int Interval::getWeight(const EPitch& ep1, const EPitch& ep2) {
    return weights[(static_cast<int>(ep2.pitch) - static_cast<int>(ep1.pitch) + 12) % 12];
}

