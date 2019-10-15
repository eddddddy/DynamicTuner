#ifndef _ALGO_H_
#define _ALGO_H_

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "hash.h"

struct EPitch;
class Tuning;
class TuningSequence;


namespace Algo {
	// Contains the main algorithms for optimizing pitch frequencies

    // Given a Tuning object that represents fixed pitches and a list of EPitch
    //   objects that represents variable pitches, return a map containing every
    //   possible way to tune the new pitches (as Tuning objects), mapping every
    //   tuning to the value of that tuning. The value of a tuning is determined
    //   (roughly) by summing the weights of all intervals that have the ideal
    //   ratio between every pair of notes.
    // The ratios in the new Tuning objects produced will be relative to the ratios
    //   of the fixed Tuning given. If the fixed Tuning does not contain any pitches,
    //   an arbitrary note in the variable pitches is chosen to be the baseline
    //   pitch.
    std::map<Tuning, int> getValuesRec(const Tuning& fixed, std::list<EPitch> var,
        std::unordered_map<EPitch, std::unordered_set<NoteTuning, Hash>, Hash> calculated =
        std::unordered_map<EPitch, std::unordered_set<NoteTuning, Hash>, Hash>());

	// Given a Tuning object that represents fixed pitches and a list of EPitch
	//   objects that represents variable pitches, return a multimap that maps
	//   an integer to every Tuning object that has that value. See getValuesRec
	//   for the specifications of the Tuning objects produced.
    std::multimap<int, Tuning> getValues(const Tuning& fixed, const std::list<EPitch>& var);

	// Given a Tuning object that represents fixed pitches and a list of EPitch
	//   objects that represents variable pitches, return a vector of Tuning
	//   objects, each containing the variable pitches (and only those pitches),
	//   with optimal value. See getValuesRec for the specifications of the Tuning
	//   objects.
    std::vector<Tuning> getBestValues(const Tuning& fixed, const std::list<EPitch>& var);

	// Internal function
    std::vector<TuningSequence> getTuningsRec(std::multimap<int, Tuning> prev, std::list<std::list<EPitch>> next, int& value, unsigned int trim = 0);

	// Returns a vector of TuningSequences with optimal value given a sequence of
	//   pitches
	// The sequence of pitches is a list of lists of pitches in the following
	//   format: every element of the outer list is a new collection of
	//   simultaneous notes directly following the previous collection of
	//   simultaneous notes (if one exists). Note that the collections do not
	//   need to fall on consistent beats. Every element of an inner list
	//   is a note in the corresponding collection.
	//   For example, if the score looks like the following:
	/*
			G      A                      G  
			E	   F                      E
	        C ---- C                D           
	*/
	//     where dashed lines represent ties and spaces after notes
	//     represent the length of the note, then the sequence of pitches
	//     can be represented by
	//       list(list(C, E, G), list(C, F, A), list(D), list(E, G))
	// If the second argument is passed in and is non-null, the value of the
	//   pointer will be set to the value of the optimal tuning sequences.
    std::vector<TuningSequence> getTunings(std::list<std::list<EPitch>> seq, int* value = nullptr);
}

#endif
