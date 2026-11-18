#pragma once
#include <string>

#if defined(_WIN32) && defined(FRACTION_DLL)
# if defined(FRACTION_EXPORTS)
#  define FRACTION_API __declspec(dllexport)
# else
#  define FRACTION_API __declspec(dllimport)
# endif
#else
# define FRACTION_API
#endif

class FRACTION_API Fraction {
    long whole_ = 0;
    unsigned short fraction_ = 0;
    static int live_;
public:
    enum { SCALE = 10000 };
    Fraction() noexcept;
    Fraction(long whole, unsigned short fraction);
    Fraction(const Fraction& other) noexcept;
    Fraction& operator=(const Fraction&) = default;
    ~Fraction() noexcept;
    static int Live() noexcept;
    void Init(long whole, unsigned short fraction);
    void Read();
    void Display() const;
    std::string toString() const;
    long long scaled() const;
    static Fraction FromScaled(long long units);
    Fraction add(const Fraction& b) const;
    Fraction sub(const Fraction& b) const;
    Fraction mul(const Fraction& b) const;
    bool equal(const Fraction& b) const;
    bool less(const Fraction& b) const;
    bool greater(const Fraction& b) const;
    Fraction operator+(const Fraction& b) const;
    Fraction operator-(const Fraction& b) const;
    Fraction operator*(const Fraction& b) const;
    bool operator==(const Fraction& b) const;
};
