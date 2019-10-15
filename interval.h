#ifndef _INTERVAL_H_
#define _INTERVAL_H_

class Frac;
class EPitch;

class Interval {
	// Class that represents an interval between two pitches and provides
	//   two methods to get the interval's ideal ratio and weight, intended
	//   for use by the optimization algorithms in this project
    private:
        static Frac idealRatios[12];
        static int weights[12];
		
    public:
		// For the two methods below, the interval is the distance between the
		//   given start and end pitches. If the distance between them is 
		//   larger than one octave or if the interval is inverted, adjust
		//   the interval accordingly so that it is between a perfect unison
		//   (the smallest interval) and a major seventh (the largest interval)

		// Returns a Frac object corresponding to the ideal ratio to tune the
		//   interval in, according to Pythagorean theory. For example, if the
		//   current interval object represents a minor sixth, the ratio would
		//   be 8/5
        static Frac getIdealRatio(const EPitch& start, const EPitch& end);
		
		// Returns an integer representing the importance of the interval
		//   relative to other intervals, where importance is defined
		//   subjectively based on how pleasant it sounds to the human ear.
		//   Certain intervals such as a perfect unison or a perfect fifth
		//   sound pleasant and hence have a very large weight, whereas other
		//   intervals such as minor seconds or tritones sound dissonant and
		//   are near-disposable.
        static int getWeight(const EPitch& start, const EPitch& end);
};

#endif
