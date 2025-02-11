#ifndef LONGNUMBER_1_HPP
#define LONGNUMBER_1_HPP

#include <iostream>
#include <vector>
#include <algorithm>

#define explode exit

class LongNumber {
private:
    std::vector<unsigned> digits;
    unsigned fractional_bits;
    bool sign = false;

    static unsigned divide_up(unsigned a, unsigned b);
    bool bit(unsigned n) const;

public:
    static LongNumber make_number(long double number);
    LongNumber();
    LongNumber(unsigned fractional_bits);
    LongNumber(const LongNumber& number);
    LongNumber(long double number);

    void set_fractional_bits(unsigned b);

    LongNumber& operator=(const LongNumber& number);

    bool operator==(const LongNumber& number);
    bool operator!=(const LongNumber& number);
    bool operator>(const LongNumber& number);
    bool operator<(const LongNumber& number);

    LongNumber operator-() const;
    LongNumber operator+(const LongNumber& number) const;
    LongNumber operator-(const LongNumber& number) const;
    LongNumber operator*(const LongNumber& number) const;
    LongNumber operator/(const LongNumber& number) const;
    friend LongNumber operator ""_longnum(long double number);
    ~LongNumber();
};

#endif // LONGNUMBER_1_HPP