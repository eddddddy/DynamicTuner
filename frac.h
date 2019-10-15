#ifndef _FRAC_H_
#define _FRAC_H_

#include <iostream>

class Frac {
	// Class that stores rational numbers and provides utility methods for
	//   manipulation. Intended to be used for representing ideal ratios
	//   between two pitches
    private:
        Frac& reduce();
        Frac(unsigned long, unsigned long, bool);
		
    public:
		// The numerator and denominator, respectively. After initialization,
		//   these will always be coprime
        unsigned long p, q;
		
		// Constructs a Frac object with the given numerator and denominator.
		//   Both integers provided must be positive.
        Frac(unsigned long, unsigned long);
		
		// Returns a new Frac object between 1 (inclusive) and 2 (exclusive).
		//   The new fraction will be constructed from the current one by scaling
		//   it by some factor of 2. (Note that this new object is well-defined.)
        Frac adjust() const;
		
		// Multiplies the current fraction in place by 2 raised to the power of
		//   the offset
        void adjust(int offset);
		
		// Returns a new Frac object obtained by multiplying the current object
		//   by other (using standard multiplication over the real numbers)
        Frac operator*(const Frac& other) const;
        Frac operator*(const int other) const;
		
		// Returns a double obtained by multiplying the current fraction by other
        double operator*(double other) const;
		
		// Returns a new Frac object obtained by dividing the current object by
		//   other
        Frac operator/(const Frac& other) const;
        Frac operator/(const int other) const;
		
		// Returns true if two Frac objects have the same numerator and denominator,
		//   and false otherwise. Note that this is the standard notion of equality
		//   since Frac objects are guaranteed to be in lowest terms.
        bool operator==(const Frac& other) const;
		
		// Returns true if the current Frac object is less than other, using the
		//   standard order relation over the real numbers, and false otherwise
        bool operator<(const Frac& other) const;
};

// Returns a double obtained by multiplying a double with the given Frac object,
//   using standard multiplication over the real numbers
double operator*(double, const Frac&);

// Returns a double obtained by dividing a double with the given Frac object, using
//   standard division over the real numbers
double operator/(double, const Frac&);

// Output operator for Frac objects
std::ostream& operator<<(std::ostream&, const Frac&);

#endif
