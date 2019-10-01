#include <iostream>

#include "frac.h"

unsigned long gcd(unsigned long a, unsigned long b) {
    while (b) {
        unsigned long t = a;
        a = b;
        b = t % b;
    }
    return a;
}

Frac& Frac::reduce() {
    unsigned long d = gcd(p, q);
    p /= d;
    q /= d;
    return *this;
}

Frac::Frac(unsigned long p, unsigned long q): p{p}, q{q} {
    reduce();
}

Frac Frac::adjust() const {
    unsigned long cp = p, cq = q;
    while (cp < cq) {
        if (cq % 2 == 0) {
            cq = cq >> 1;
        } else {
            cp = cp << 1;
        }
    }
    while (cp >= 2 * cq) {
        if (cp % 2 == 0) {
            cp = cp >> 1;
        } else {
            cq = cq << 1;
        }
    }
    return Frac{cp, cq};
}

void Frac::adjust(int offset) {
    if (offset >= 0) {
        p = p << offset;
    } else {
        q = q << (-1 * offset);
    }
    reduce();
}

Frac Frac::operator*(const Frac& other) const {
    unsigned long d1 = gcd(p, other.q);
    unsigned long d2 = gcd(q, other.p);
    return Frac{p / d1 * other.p / d2, q / d2 * other.q / d1};
}

Frac Frac::operator*(const int other) const {
    unsigned long d = gcd(q, other);
    return Frac{other / d * p, q / d};
}

double Frac::operator*(double other) const {
    return other * p / q;
}

Frac Frac::operator/(const Frac& other) const {
    unsigned long d1 = gcd(p, other.p);
    unsigned long d2 = gcd(q, other.q);
    return Frac{p / d1 * other.q / d2, q / d2 * other.p / d1};
}

Frac Frac::operator/(const int other) const {
    unsigned long d = gcd(p, other);
    return Frac{p / d, other / d * q};
}

bool Frac::operator==(const Frac& other) const {
    return (p == other.p) && (q == other.q);
} 

bool Frac::operator<(const Frac& other) const {
    return p * other.q < q * other.p;
}

double operator*(double d, const Frac& f) {
    return d * f.p / f.q;
}

double operator/(double d, const Frac& f) {
    return d * f.q / f.p;
}

std::ostream& operator<<(std::ostream& out, const Frac& f) {
    out << f.p << "/" << f.q;
    return out;
}
