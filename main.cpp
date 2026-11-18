#include "fraction.h"
#include <climits>
#include <iostream>
#include <stdexcept>

void Require(bool condition) {
    if (!condition) throw std::logic_error("Self-check failed");
}

template<class Error, class Action>
void RequireThrows(Action action) {
    bool caught = false;
    try { action(); }
    catch (const Error&) { caught = true; }
    Require(caught);
}

void CheckLifecycle() {
    const int before = Fraction::Live();
    {
        Fraction zero;
        Fraction value(1, 2500);
        Fraction copy(value);
        Require(Fraction::Live() == before + 3);
        Require(zero.scaled() == 0 && copy == value);
        std::cout << "Default constructor: " << zero.toString() << '\n';
        std::cout << "Parameter constructor: " << value.toString() << '\n';
        std::cout << "Copy constructor: " << copy.toString() << '\n';
    }
    Require(Fraction::Live() == before);
    std::cout << "Destructor check: PASS\n";
}

void SelfCheck() {
    const Fraction a(2, 5000), b(-2, 7500);
    Require((a + b).scaled() == 12500);
    Require((a - b).scaled() == 37500);
    Require((a * b).scaled() == -31250);
    Require(a == a && !(a == b));
    Require(b.less(a) && a.greater(b));
    Require(a.add(b) == a + b && a.sub(b) == a - b && a.mul(b) == a * b);
    const Fraction negative(-1, 7500);
    Require(negative.scaled() == -2500 && negative.toString() == "-0.2500");
    Require((Fraction::FromScaled(1) * Fraction::FromScaled(5000)).scaled() == 1);
    Require((Fraction::FromScaled(-1) * Fraction::FromScaled(5000)).scaled() == -1);
    Require((Fraction::FromScaled(1) * Fraction::FromScaled(4900)).scaled() == 0);
    const long long maxTicks = static_cast<long long>(LONG_MAX) * Fraction::SCALE
                            + Fraction::SCALE - 1;
    const long long minTicks = static_cast<long long>(LONG_MIN) * Fraction::SCALE;
    RequireThrows<std::invalid_argument>([] { Fraction invalid(0, 10000); });
    RequireThrows<std::overflow_error>([&] {
        Fraction result = Fraction::FromScaled(maxTicks) + Fraction::FromScaled(1);
        (void)result;
    });
    RequireThrows<std::overflow_error>([&] {
        Fraction result = Fraction::FromScaled(minTicks) - Fraction::FromScaled(1);
        (void)result;
    });
    RequireThrows<std::overflow_error>([&] {
        Fraction result = Fraction::FromScaled(maxTicks) * Fraction(2, 0);
        (void)result;
    });
    Require(a.scaled() == 25000 && b.scaled() == -12500);
    std::cout << "Arithmetic, comparisons, rounding and overflow checks: PASS\n";
}

int main() {
    try {
        CheckLifecycle();
        SelfCheck();
        Fraction a, b;
        std::cout << "Enter whole and fractional fields for a: "; a.Read();
        std::cout << "Enter whole and fractional fields for b: "; b.Read();
        std::cout << "a = "; a.Display();
        std::cout << "b = "; b.Display();
        std::cout << "a + b = " << (a + b).toString() << '\n';
        std::cout << "a - b = " << (a - b).toString() << '\n';
        std::cout << "a * b = " << (a * b).toString() << '\n';
        std::cout << std::boolalpha << "a == b: " << (a == b) << '\n';
        std::cout << "a < b (less): " << a.less(b) << '\n';
        std::cout << "a > b (greater): " << a.greater(b) << '\n';
        std::cout << "Stored (-1,7500) = " << Fraction(-1,7500).toString() << '\n';
        try { Fraction invalid(0, 10000); }
        catch (const std::invalid_argument& error) {
            std::cout << "Expected error: " << error.what() << '\n';
        }
        std::cout << "Self-checks: PASS\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
