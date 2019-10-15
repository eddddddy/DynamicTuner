#include <iostream>

#include "frac.h"

unsigned long gcd(unsigned long u, unsigned long v) {
    int shift;
    if (u == 0) return v;
    if (v == 0) return u;
    shift = __builtin_ctzl(u | v);
    u >>= __builtin_ctzl(u);
    do {
        v >>= __builtin_ctzl(v);
        if (u > v) {
            unsigned long t = v;
            v = u;
            u = t;
        }  
        v = v - u;
    } while (v != 0);
    return u << shift;
}

Frac& Frac::reduce() {
    unsigned long d = gcd(p, q);
    p /= d;
    q /= d;
    return *this;
}

Frac::Frac(unsigned long p, unsigned long q, bool b): p{p}, q{q} {}

Frac::Frac(unsigned long p, unsigned long q): p{p}, q{q} {
    reduce();
}

Frac Frac::adjust() const {
    unsigned long u = p >> __builtin_ctzl(p);
    unsigned long v = q >> __builtin_ctzl(q);
    if (u > v) {
        v <<= (__builtin_clzl(v) - __builtin_clzl(u));
        v >>= (v > u);
    } else if (u < v) {
        u <<= (__builtin_clzl(u) - __builtin_clzl(v));
        u <<= (v > u);
    }
    return Frac{u, v, true};
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
    return Frac{p / d1 * other.p / d2, q / d2 * other.q / d1, true};
}

Frac Frac::operator*(const int other) const {
    unsigned long d = gcd(q, other);
    return Frac{other / d * p, q / d, true};
}

double Frac::operator*(double other) const {
    return other * p / q;
}

Frac Frac::operator/(const Frac& other) const {
    unsigned long d1 = gcd(p, other.p);
    unsigned long d2 = gcd(q, other.q);
    return Frac{p / d1 * other.q / d2, q / d2 * other.p / d1, true};
}

Frac Frac::operator/(const int other) const {
    unsigned long d = gcd(p, other);
    return Frac{p / d, other / d * q, true};
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
