#ifndef _TUNINGS_H_
#define _TUNINGS_H_

#include <iostream>
#include <vector>
#include <list>
#include <set>

#include "frac.h"
#include "pitch.h"

class TuningSequence;

struct NoteTuning {
	// Struct that represents a way to tune a certain pitch: stores
	//   an EPitch object and a Frac representing how the pitch is
	//   to be tuned. Note that the ratio is only relevant in two contexts:
	//   when comparing to other NoteTunings and when using a relative
	//   pitch to get a frequency for the pitch.
    EPitch pitch;
    Frac tuning;

	// Returns an EPitchFreq object containing the frequency (in Hz)
	//   to tune the pitch, relative to the double given. The frequency
	//   is calculated by multiplying relFreq with the stored tuning ratio.
    EPitchFreq getEPitchFreq(double relFreq) const;
};

// Returns true if either the pitch of the NoteTuning on the left
//   is lower than the pitch of the NoteTuning on the right, or
//   otherwise if the pitches are equal and the tuning of the
//   NoteTuning on the left is less than the tuning of the NoteTuning
//   on the right. Returns false otherwise.
bool operator<(const NoteTuning&, const NoteTuning&);

// Returns true if both NoteTuning objects have the same pitch
//   and the same tuning, and false otherwise
bool operator==(const NoteTuning&, const NoteTuning&);

// Returns true if the NoteTuning has pitch equal to the given
//   pitch, and false otherwise
bool operator==(const NoteTuning&, const EPitch&);
bool operator==(const EPitch&, const NoteTuning&);

class Tuning {
	// Class that represents a collection of simultaneously sounding
	//   NoteTuning objects. Provides methods for adding/removing
	//   NoteTunings and an iterator to iterate over them.
	// Note again that the ratios only have meaning in context: either
	//   when comparing Tunings with other Tunings, or when calculating
	//   the frequencies of the NoteTunings in the Tuning using a relative
	//   frequency.
    private:
        std::multiset<NoteTuning> noteTunings;

    public:
		// Create a Tuning object, initially empty
        Tuning();
		
		// Add a NoteTuning to the Tuning, allowing duplicate pitches but
		//   not duplicate tunings for those pitches. Returns itself for 
		//   chaining.
        Tuning& addNoteTuning(const NoteTuning&);
		
		// Remove a NoteTuning from the Tuning if it exists. Return true
		//   if the NoteTuning was removed, and false otherwise. 
        bool removeNoteTuning(const NoteTuning&);
		
		// Remove a NoteTuning from the Tuning by its pitch, if one exists.
		//   If there is more than one NoteTuning, remove an arbitrary one.
		//   Return the NoteTuning that was removed if successful, otherwise
		//   return a sentinel NoteTuning with its tuning ratio set to 0.
        NoteTuning removePitch(const EPitch&);
		
		// Returns true if the Tuning object is empty (i.e. no NoteTunings
		//   have been added that have not been removed), otherwise return
		//   false
        bool isEmpty() const;
		
		// Modifies the current Tuning object so that every NoteTuning with
		//   a pitch in the filter list is removed from it and added to a
		//   new Tuning object, which is then returned. If there are multiple
		//   NoteTunings that have the same pitch, then only one will be removed
		//   (unless that pitch occurs multiple times in filter).
		// The filter list must only contain pitches that are in the current
		//   Tuning, otherwise behaviour is undefined.
        Tuning split(const std::list<EPitch>& filter);

		// Returns a vector of EPitchFreq objects containing the frequencies
		//   (in Hz) to tune the pitches in every NoteTuning in the Tuning,
		//   relative to the double given. The vector is ordered from lowest
		//   to highest using the < operator for NoteTunings.
		// Equivalent to iterating over every NoteTuning, calling
		//   getEPitchFreq using the same relFreq passed in, and appending
		//   the resulting EPitchFreq to the end of an accumulating vector.
        std::vector<EPitchFreq> getEPitchFreqs(double relFreq) const;
		
		class TuningIter {
			// Iterator class for iterating over specific note tunings
			//   in the tuning
            private:
                std::multiset<NoteTuning>::iterator it;
                TuningIter(std::multiset<NoteTuning>::iterator it);
            public:
				// Operators to support range-based for loops. See below
				//   for documentation.
                Tuning::TuningIter& operator++();
                NoteTuning operator*() const;
                bool operator!=(const Tuning::TuningIter& other) const;

            friend Tuning;
        };

		// Create an iterator that iterates over every NoteTuning in the Tuning,
		//   ordered from lowest to highest using the < comparison between two
		//   NoteTuning objects.
		//   Dereferencing the iterator will produce the current NoteTuning.
		//   Incrementing the iterator will redirect it to the next NoteTuning.
        TuningIter begin() const;
		
		// End condition for iteration. Compare this to an iterator using
		//   the not-equals operator.
        TuningIter end() const;

		// Return a new Tuning object containing every NoteTuning in the
		//   left operand in addition to the NoteTuning that is the right
		//   operand
		// Equivalent to copying the Tuning object and calling addNoteTuning
		//   on the new object
        Tuning operator+(const NoteTuning&) const;
		
		// Return a new TuningSequence object containing every Tuning in
		//   the right operand in addition to the Tuning that is the left
		//   operand appended to the start of it
		// Equivalent to copying the TuningSequence object and calling
		//   addTuningFront on the new object
        TuningSequence operator+(const TuningSequence&) const;
		
		// Returns true if the Tuning on the left is less than the Tuning on
		//   the right, and false otherwise. Between two Tuning objects, the one
		//   with the smallest NoteTuning with respect to the < operator for
		//   NoteTunings is considered the smaller one. This comparison continues
		//   lexicographically.
        bool operator<(const Tuning&) const;
};

class TuningSequence {
	// Class that represents a sequence of NoteTunings over time, some of which
	//   may be simultaneous (e.g. a chord progression); because of this, the
	//   Tunings maintain the order in which they are inserted. Provides methods
	//   for adding Tunings to the start or the end, and an iterator to iterate
	//   over them in order.
    private:
        std::list<Tuning> tunings;

    public:
		// Create a TuningSequence object, initially empty
        TuningSequence();
		
		// Add a Tuning to the end of the TuningSequence, returning itself for
		//   chaining
        TuningSequence& addTuning(const Tuning&);
		
		// Add a Tuning to the start of the TuningSequence, return itself for
		//   chaining
        TuningSequence& addTuningFront(const Tuning&);

		// Returns a vector of vectors of EPitchFreq objects containing the
		//   the frequencies (in Hz) to tune the pitches in every NoteTuning
		//   in every Tuning in the TuningSequence. The frequencies are made
		//   to correspond as closely as possible to the equal-temperament
		//   frequencies at first, but the frequencies may diverge depending
		//   on the Tunings in the sequence.
		// Equivalent to iterating over every Tuning, calling getEPitchFreqs
		//   using some relFreq passed in, and appending the resulting vector
		//   of EPitchFreq objects to the end of another accumulating vector.
		//   The relative frequency is calculated by taking the equal-temperament
		//   frequency of the EPitch of the lowest NoteTuning of the first
		//   Tuning in the TuningSequence, and dividing that by the tuning
		//   ratio of the same NoteTuning.
        std::vector<std::vector<EPitchFreq>> getFreqs() const;
		
		class TuningSequenceIter {
			// Iterator class for iterating over specific tunings in
			//   the tuning sequence
            private:
                std::list<Tuning>::const_iterator it;
                TuningSequenceIter(std::list<Tuning>::const_iterator it);
            public:
				// Operators to support range-based for loops. See below for
				//   documentation.
                TuningSequence::TuningSequenceIter& operator++();
                Tuning operator*() const;
                bool operator!=(const TuningSequence::TuningSequenceIter& other) const;
        
            friend TuningSequence;
        };

		// Create an iterator that iterates over every Tuning in the TuningSequence
		//   in order. This order is well-defined based on addTuning and
		//   addTuningFront calls.
		//   Dereferencing the iterator will produce the current Tuning object.
		//   Incrementing the iterator will redirect it to the next Tuning.
        TuningSequenceIter begin() const;
		
		// End condition for iteration. Compare this to an iterator using
		//   the not-equals operator.
        TuningSequenceIter end() const;

		// Returns true if the TuningSequence on the left is less than the
		//   TuningSequence on the right, and false otherwise. Between two
		//   TuningSequence objects, the one with the smallest Tuning at the
		//   front with respect to the < operator for Tunings is considered the
		//   smaller one. This comparison continues lexicographically.
        bool operator<(const TuningSequence&) const;
};

// Returns the standard equal-temperament frequency of the given pitch object
double getNormalFreq(const EPitch&);

// Output operator for NoteTuning objects
std::ostream& operator<<(std::ostream&, const NoteTuning&);

// Output operator for Tuning objects
std::ostream& operator<<(std::ostream&, const Tuning&);

// Output operator for TuningSequence objects
std::ostream& operator<<(std::ostream&, const TuningSequence&);

#endif
