#include "fraction.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

// Windows GCC/MinGW and MSVC use a 32-bit signed long.
static_assert(std::numeric_limits<long>::digits <= 31, "A 32-bit long is required");
int Fraction::live_ = 0;
Fraction::Fraction() noexcept { ++live_; }
Fraction::Fraction(long whole, unsigned short fraction) {
    Init(whole, fraction);
    ++live_;
}
Fraction::Fraction(const Fraction& other) noexcept
    : whole_(other.whole_), fraction_(other.fraction_) { ++live_; }
Fraction::~Fraction() noexcept { --live_; }
int Fraction::Live() noexcept { return live_; }

void Fraction::Init(long whole, unsigned short fraction) {
    if (fraction >= SCALE) throw std::invalid_argument("Fraction must be 0..9999");
    whole_ = whole; fraction_ = fraction;
}
void Fraction::Read() {
    long whole; int fraction;
    if (!(std::cin >> whole >> fraction)) throw std::runtime_error("Invalid input");
    if (fraction < 0 || fraction >= SCALE)
        throw std::invalid_argument("Fraction must be 0..9999");
    Init(whole, static_cast<unsigned short>(fraction));
}
void Fraction::Display() const { std::cout << toString() << '\n'; }
long long Fraction::scaled() const {
    return static_cast<long long>(whole_) * SCALE + fraction_;
}
std::string Fraction::toString() const {
    long long units = scaled();
    unsigned long long magnitude = static_cast<unsigned long long>(units < 0 ? -units : units);
    std::ostringstream out;
    if (units < 0) out << '-';
    out << magnitude / SCALE << '.' << std::setw(4) << std::setfill('0') << magnitude % SCALE;
    return out.str();
}
Fraction Fraction::FromScaled(long long units) {
    long long whole = units / SCALE, fraction = units % SCALE;
    if (fraction < 0) { --whole; fraction += SCALE; }
    if (whole < std::numeric_limits<long>::min() || whole > std::numeric_limits<long>::max())
        throw std::overflow_error("Fraction result overflow");
    Fraction result;
    result.Init(static_cast<long>(whole), static_cast<unsigned short>(fraction));
    return result;
}
Fraction Fraction::add(const Fraction& b) const { return FromScaled(scaled() + b.scaled()); }
Fraction Fraction::sub(const Fraction& b) const { return FromScaled(scaled() - b.scaled()); }
Fraction Fraction::mul(const Fraction& b) const {
    long long x = scaled(), y = b.scaled();
    bool negative = (x < 0) != (y < 0);
    unsigned long long a = static_cast<unsigned long long>(x < 0 ? -x : x);
    unsigned long long c = static_cast<unsigned long long>(y < 0 ? -y : y);
    if (a == 0 || c == 0) return FromScaled(0);
    unsigned long long limit = negative
        ? static_cast<unsigned long long>(-static_cast<long long>(std::numeric_limits<long>::min()) * SCALE)
        : static_cast<unsigned long long>(std::numeric_limits<long>::max()) * SCALE + SCALE - 1;
    unsigned long long whole = a / SCALE;
    if (whole > limit / c) throw std::overflow_error("Fraction result overflow");
    unsigned long long units = whole * c;
    // The product here is bounded by 9999 * (2^31 * 10000).
    unsigned long long tail = ((a % SCALE) * c + SCALE / 2) / SCALE;
    if (tail > limit - units) throw std::overflow_error("Fraction result overflow");
    units += tail;
    return FromScaled(negative ? -static_cast<long long>(units) : static_cast<long long>(units));
}
bool Fraction::equal(const Fraction& b) const { return scaled() == b.scaled(); }
bool Fraction::less(const Fraction& b) const { return scaled() < b.scaled(); }
bool Fraction::greater(const Fraction& b) const { return scaled() > b.scaled(); }

Fraction Fraction::operator+(const Fraction& b) const { return add(b); }
Fraction Fraction::operator-(const Fraction& b) const { return sub(b); }
Fraction Fraction::operator*(const Fraction& b) const { return mul(b); }
bool Fraction::operator==(const Fraction& b) const { return equal(b); }
