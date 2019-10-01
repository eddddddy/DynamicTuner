#ifndef _SCORE_H_
#define _SCORE_H_

#include <vector>
#include <list>
#include <map>

#include "pitch.h"

struct Note {
	// Struct that packages a pitch frequency, the duration of that
	//   frequency, and the start beat of that frequency
    const EPitchFreq pitch;
    int duration;
    int startBeat;
};

class Score {
	// Class that represents a musical score and uses the algorithms in
	//   this project to optimize how pitches in the score are tuned.
	//   Also provides iterators to iterate over the beats or the notes
	//   in the score.

    private:
        std::vector<std::map<EPitchFreq, bool>> score;
        std::vector<std::list<EPitchFreq>> notes;
        std::vector<std::list<EPitch>> pitches;

    public:
		// Create an empty Score
        Score();

		// Get the length of the Score. The length of the score is the
		//   beat on which the last note is released, minus 1.
        int getLength() const;

		// Add a note to the score. The frequency field of the note's
		//   EPitchFreq object is ignored. Returns itself for chaining.
        Score& add(const Note&);
		
		// Add a pitch to the score with the specified duration and start
		//   beat. Returns itself for chaining.
        Score& add(const EPitch&, uint32_t duration, uint32_t beat);

		// Internally calculates the optimal frequencies of the pitches
		//   in the score. This method must be called before any iterators
		//   are instantiated, otherwise the behaviour is undefined. Every
		//   time a new note is added, this method must be called again
		//   before instantiating iterators.
        void calculateFreqs();

        class BeatIter {
			// Iterator class for iterating over beats in the score
            private:
                std::vector<std::list<EPitchFreq>>::const_iterator it;
                BeatIter(std::vector<std::list<EPitchFreq>>::const_iterator);
            public:
				// Operators to support range-based for loops. See below for
				//   documentation
                Score::BeatIter& operator++();
                std::list<EPitchFreq> operator*() const;
                bool operator!=(const Score::BeatIter& other) const;

            friend Score;
        };

        class NoteIter {
			// Iterator class for iterating over notes in the score
            private:
                std::vector<std::map<EPitchFreq, bool>>::const_iterator begin;
                const std::vector<std::map<EPitchFreq, bool>>::const_iterator end;
                std::map<EPitchFreq, bool>::const_iterator it;
                int beat;
                NoteIter(std::vector<std::map<EPitchFreq, bool>>::const_iterator, std::vector<std::map<EPitchFreq, bool>>::const_iterator);
            public:
				// Operators to support range-based for loops. See below for
				//   documentation
                Score::NoteIter& operator++();
                Note operator*() const;
                bool operator!=(const Score::NoteIter& other) const;

            friend Score;
        };

		// Create an iterator that iterates over every beat in the score.
		//   Dereferencing the iterator will produce a list of EPitchFreq
		//   objects representing the pitch frequencies at that beat (possibly
		//   empty).
		//   Incrementing the iterator will increment the beat.
        BeatIter begin() const;
		
		// End condition for iteration. Compare this to an iterator using
		//   the not-equals operator.
        BeatIter end() const;
		
		// Create an iterator that iterates over every note in the score.
		//   Dereferencing the iterator will produce a Note object containing
		//   data for that note.
		//   Incrementing the iterator will produce a new iterator with the
		//   next note. The notes should not be assumed to be in any particular
		//   order, but (of course) it is guaranteed that every note will
		//   be reached before the end condition is reached.
        NoteIter nbegin() const;
		
		// End condition for iteration. Compare this to an iterator using
		//   the not-equals operator.
        NoteIter nend() const;
};

// A sample score, defined in sample.cc
extern Score SAMPLE_SONG;

#endif
