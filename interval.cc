#include "frac.h"
#include "pitch.h"
#include "interval.h"

enum class Interval::Int {
    perfectUnison = 0,
    minorSecond = 1,
    majorSecond = 2,
    minorThird = 3,
    majorThird = 4,
    perfectFourth = 5,
    tritone = 6,
    perfectFifth = 7,
    minorSixth = 8,
    majorSixth = 9,
    minorSeventh = 10,
    majorSeventh = 11
};

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

int Interval::weights[12] = {1024, 2, 3, 8, 8, 32, 1, 32, 8, 8, 3, 2};

Interval::Interval(const EPitch& ep1, const EPitch& ep2):
	interval{static_cast<Int>((static_cast<int>(ep2.pitch) - static_cast<int>(ep1.pitch) + 12) % 12)} {}

Frac Interval::getIdealRatio() {
    return idealRatios[static_cast<int>(interval)];
}

int Interval::getWeight() {
    return weights[static_cast<int>(interval)];
}

