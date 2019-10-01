#ifndef _PITCH_H_
#define _PITCH_H_

#include <iostream>

// Enumeration containing the 12 pitches of a piano
enum class Pitch {C = 0, Cs = 1, D = 2, Ds = 3, E = 4, F = 5, Fs = 6, G = 7, Gs = 8, A = 9, As = 10, B = 11};

struct EPitch {
	// Struct that represents a key on a piano: stores a member of the Pitch
	//   enumeration and an octave number. The lowest note on the piano is
	//   represented by Pitch = A and octave = 0, middle C is represented by
	//   Pitch = C and octave = 4, and so on.
    Pitch pitch;
    int octave;
};

struct EPitchFreq {
	// Struct that represents a frequency to play a certain pitch: stores an
	//   EPitch object and a double. For example, middle C in the standard
	//   equal-temperament piano tuning would have a frequency of 261.626.
    EPitch pitch;
    double freq;
};

// Returns true if the pitch on the left is lower than the pitch on the right, starting
//   from C, and false otherwise
bool operator<(Pitch, Pitch);

// Returns true if the pitch on the left is lower than or the same as the pitch on the
//   right, and false otherwise
bool operator<=(Pitch, Pitch);

// Returns true if the pitch on the left is the same as the pitch on the right, and false
//   otherwise
bool operator==(Pitch, Pitch);

// Returns true if the pitch on the left is higher than the pitch on the right, and false
//   otherwise
bool operator>(Pitch, Pitch);

// Returns true if the pitch on the left is higher than or the same as the pitch on the
//   right, and false otherwise
bool operator>=(Pitch, Pitch);

// Output operator for members of the Pitch enumeration
std::ostream& operator<<(std::ostream&, Pitch);

// Returns true if the pitch on the left is lower than the pitch on the right. A pitch is
//   lower than another pitch only if either its octave is lower, or otherwise if their
//   octaves are the same and its pitch is lower. Returns false otherwise.
bool operator<(const EPitch&, const EPitch&);

// Returns true if the pitch on the left is lower than or the same as the pitch on the
//   right, and false otherwise
bool operator<=(const EPitch&, const EPitch&);

// Returns true if the pitch on the left is the same as the pitch on the right. Two pitches
//   are the same if they have the same octave and the same pitch. Returns false otherwise.
bool operator==(const EPitch&, const EPitch&);

// Returns true if the pitch on the left is higher than the pitch on the right, and false
//   otherwise
bool operator>(const EPitch&, const EPitch&);

// Returns true if the pitch on the left is higher than or the same as the pitch on the
//   right, and false otherwise
bool operator>=(const EPitch&, const EPitch&);

// Output operator for EPitch objects
std::ostream& operator<<(std::ostream&, const EPitch&);

// Returns true if the pitch of the pitch frequency on the left is lower than the pitch
//   of the pitch frequency on the right, and false otherwise
bool operator<(const EPitchFreq&, const EPitchFreq&);

// Returns true if the pitch of the pitch frequency on the left is lower than or the same
//   as the pitch of the pitch frequency on the right, and false otherwise
bool operator<=(const EPitchFreq&, const EPitchFreq&);

// Returns true if the pitch of the pitch frequency on the left is equal to the pitch of
//   of the pitch frequency on the right, and false otherwise
bool operator==(const EPitchFreq&, const EPitchFreq&);

// Returns true if the pitch of the pitch frequency on the left is higher than the pitch
//   of the pitch frequency on the right, and false otherwise
bool operator>(const EPitchFreq&, const EPitchFreq&);

// Returns true if the pitch of the pitch frequency on the left is higher than or the same
//   as the pitch of the pitch frequency on the right, and false otherwise
bool operator>=(const EPitchFreq&, const EPitchFreq&);

// Output operator for EPitchFreq objects
std::ostream& operator<<(std::ostream&, const EPitchFreq&);

#endif
