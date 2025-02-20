#ifndef LONGNUMBER_HPP
#define LONGNUMBER_HPP

#include <vector>
#include <algorithm>
#include <iostream>


class LongNumber {
private:

    std::vector<unsigned> digits;
    unsigned fractional_bits;
    bool sign = false;
    static void explode(int a);
    static unsigned divide_up(unsigned a, unsigned b);
    bool bit(unsigned n) const;
    bool operator%(const LongNumber& number) const;
    LongNumber shift_left();
    LongNumber shift_right();
    void shift_left_for_mul();
    void shift_right_for_mul();
    bool is_zero() const;

public:
    void print_data();
    static LongNumber make_number(long double number);
    LongNumber();
    LongNumber(unsigned fractional_bits);
    LongNumber(const LongNumber& number);
    
    void set_fractional_bits(unsigned b);
    LongNumber& operator=(const LongNumber& number);
    
    bool operator==(const LongNumber& number) const;
    bool operator!=(const LongNumber& number) const;
    bool operator>(const LongNumber& number) const;
    bool operator<(const LongNumber& number) const;
    
    LongNumber abs() const;
    LongNumber operator-() const;
    LongNumber operator+(const LongNumber& number) const;
    LongNumber operator-(const LongNumber& number) const;
    LongNumber operator*(const LongNumber& number) const;
    LongNumber operator/(const LongNumber& number) const;
    
    ~LongNumber() = default;
};

LongNumber operator ""_longnum(long double number);

#endif
