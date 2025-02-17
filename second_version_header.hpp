#ifndef LONGNUMBER_HPP
#define LONGNUMBER_HPP

#include <vector>
#include <algorithm>
#include <iostream>

#define explode exit

class LongNumber {
private:
    std::vector<unsigned> digits;
    unsigned fractional_bits;
    bool sign = false;
    
    static unsigned divide_up(unsigned a, unsigned b);
    bool bit(unsigned n) const;
    LongNumber operator%(const LongNumber& number) const;

public:
    static LongNumber make_number(long double number);
    LongNumber();
    LongNumber(unsigned fractional_bits);
    LongNumber(const LongNumber& number);
    
    void set_fractional_bits(unsigned b);
    LongNumber& operator=(const LongNumber& number);
    
    bool operator==(const LongNumber& number);
    bool operator!=(const LongNumber& number);
    bool operator>(const LongNumber& number);
    bool operator<(const LongNumber& number);
    
    LongNumber operator-() const;
    //LongNumber operator>>(int shift) const;
    LongNumber operator+(const LongNumber& number) const;
    LongNumber operator-(const LongNumber& number) const;
    LongNumber operator*(const LongNumber& number) const;
    LongNumber operator/(const LongNumber& number) const;
    
    ~LongNumber() = default;
};

LongNumber operator ""_longnum(long double number);

#endif