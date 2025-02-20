#include "second_version_header.hpp"
#include <stdexcept>

void LongNumber::print_data() {
    std::cout << fractional_bits << '\n';
    for(auto t : digits)
        std::cout << t << ' ';
    std::cout << '\n';
}

bool LongNumber::is_zero() const {
    for(auto t : digits) {
        if(t != 0) {
            return false;
        }
    }
    return true;
}

void LongNumber::explode(int a) {
    std::cout << "We have an error!\n";
    exit(a);
}
unsigned LongNumber::divide_up(unsigned a, unsigned b) {
    return (a % b == 0) ? a / b : a / b + 1;
}

bool LongNumber::bit(unsigned n) const {
    return (digits[n / 32] & (1 << (n % 32))) != 0;
}
LongNumber LongNumber::shift_left() {
    bool carry = false;
    LongNumber result = *this;
    
    unsigned n = digits.size();
    for (unsigned i = 0; i < n; ++i) {
        bool new_carry = (result.digits[i] & (1 << (sizeof(unsigned)*8-1))) != 0;
        result.digits[i] <<= 1;
        result.digits[i] += carry;
        carry = new_carry;
    }
    if (carry) {
        explode(1);
    }
    return result;
}
LongNumber LongNumber::shift_right() {
    LongNumber result = *this;
    bool carry = false;
    for(unsigned i = 0; i < result.digits.size() - 1; ++i) {
        result.digits[i] >>= 1;
        if (result.digits[i+1] & 1) {
            result.digits[i] |= (1<<31);
        }
    }
    result.digits.back() >>= 1;
    return result;
}

void LongNumber::shift_left_for_mul() {
    unsigned need = digits.size();
    for(unsigned i = 0; i < need; ++i) {
        digits.push_back(0);
    }
    fractional_bits = fractional_bits * 2 + sizeof(unsigned)*8;
}
void LongNumber::shift_right_for_mul() {
    unsigned need = digits.size() / 2;
    digits.pop_back();
    std::reverse(digits.begin(), digits.end());
    while(digits.size() > need)
        digits.pop_back();
    fractional_bits = (fractional_bits - sizeof(unsigned)*8)/2;
    std::reverse(digits.begin(), digits.end());
}

bool LongNumber::operator%(const LongNumber& number) const {
    if (sign != number.sign) {
        explode(1);
    }
    LongNumber result = *this;
    if (fractional_bits != number.fractional_bits) {
        explode(1);
    }
    unsigned n = digits.size();
    bool carry = false;
    for (unsigned i = 0; i < n; ++i) {
        unsigned prev_result = result.digits[i];
        result.digits[i] -= number.digits[i];
        if (carry) {
            --result.digits[i];
            carry = false;
        }
        if (result.digits[i] > prev_result) {
            carry = true;
        }
    }
    return carry;
}

LongNumber LongNumber::make_number(long double number) {
    const unsigned precision = 32;
        LongNumber res = LongNumber(precision);
        if (number < 0) {
            res.sign = true;
            number *= -1;
        }
        if (number >= (1ll << 32ll)) {
            explode(1);
        }
        res.digits.back() = (unsigned)number;
        number -= (unsigned)number;
        res.digits.front() = (unsigned)(number * (1ll << 32ll));
        return res;
}

LongNumber::LongNumber() : fractional_bits(0), sign(false) {}

LongNumber::LongNumber(unsigned fractional_bits) {
    digits.assign(divide_up(fractional_bits, sizeof(unsigned) * 8) + 1, 0);
    this->fractional_bits = fractional_bits;
}

LongNumber::LongNumber(const LongNumber& number) {
    this->digits = number.digits;
    this->fractional_bits = number.fractional_bits;
    this->sign = number.sign;
}

void LongNumber::set_fractional_bits(unsigned b) {
    unsigned desired = divide_up(b, sizeof(unsigned) * 8) + 1;
        std::reverse(digits.begin(), digits.end());
        while (desired > digits.size()) {
            digits.push_back(0);
        }
        while (desired < digits.size()) {
            digits.pop_back();
        }
        fractional_bits = b;
        std::reverse(digits.begin(), digits.end());
}

LongNumber& LongNumber::operator=(const LongNumber& number) {
    this->digits = number.digits;
        this->fractional_bits = number.fractional_bits;
        this->sign = number.sign;
        return *this;
}

bool LongNumber::operator==(const LongNumber& number) const {
    if (fractional_bits != number.fractional_bits) {
        LongNumber normalizedA = *this;
        LongNumber normalizedB = number;
        if (normalizedA.fractional_bits > normalizedB.fractional_bits) {
            normalizedB.set_fractional_bits(normalizedA.fractional_bits);
        }
        else if (normalizedA.fractional_bits < normalizedB.fractional_bits) {
            normalizedA.set_fractional_bits(normalizedB.fractional_bits);
        }
        return normalizedA == normalizedB;
    }
    if (is_zero() && number.is_zero())
        return true;
    return (this->sign == number.sign && this->digits == number.digits);
}

bool LongNumber::operator!=(const LongNumber& number) const {
    return !(*this == number);
}

bool LongNumber::operator>(const LongNumber& number) const {
    if (*this == number) {
        return false;
    }
    return !((*this - number).sign);
}

bool LongNumber::operator<(const LongNumber& number) const {
    return number > *this;
}

LongNumber LongNumber::operator-() const {
    LongNumber result = *this;
    result.sign = !result.sign;
    return result;
}


LongNumber LongNumber::abs() const {
    LongNumber result = *this;
    result.sign = false;
    return result;
}

LongNumber LongNumber::operator+(const LongNumber& number) const {
    if (sign != number.sign) {
        return *this - (-number);
    }
    LongNumber result = *this;
    if (fractional_bits != number.fractional_bits) {
        LongNumber normalizedA = *this;
        LongNumber normalizedB = number;
        if (normalizedA.fractional_bits > normalizedB.fractional_bits) {
            normalizedB.set_fractional_bits(normalizedA.fractional_bits);
        }
        else {
            normalizedA.set_fractional_bits(normalizedB.fractional_bits);
        }
        return normalizedA + normalizedB;
    }
    unsigned n = digits.size();
    bool carry = false;
    bool nonzero = false;
    for (unsigned i = 0; i < n; ++i) {
        unsigned prev_result = result.digits[i];
        result.digits[i] += number.digits[i];
        if (carry) {
            ++result.digits[i];
            carry = false;
        }
        if (result.digits[i] < prev_result) {
            carry = true;
        }
        if (result.digits[i] != 0) {
            nonzero = true;
        }
    }
    if (!nonzero) {
        result.sign = false;
    }
    if (carry) {
        explode(1);
    }
    return result;
}


LongNumber LongNumber::operator-(const LongNumber& number) const {
    if (sign != number.sign) {
        return *this + (-number);
    }
    LongNumber result = *this;
    if (fractional_bits != number.fractional_bits) {
        LongNumber normalizedA = *this;
        LongNumber normalizedB = number;
        if (normalizedA.fractional_bits > normalizedB.fractional_bits) {
            normalizedB.set_fractional_bits(normalizedA.fractional_bits);
        }
        else {
            normalizedA.set_fractional_bits(normalizedB.fractional_bits);
        }
        return normalizedA - normalizedB;
    }
    if (*this%number)
        return -(number-*this);
    unsigned n = digits.size();
    bool carry = false;
    bool nonzero = false;
    for (unsigned i = 0; i < n; ++i) {
        unsigned prev_result = result.digits[i];
        result.digits[i] -= number.digits[i];
        if (carry) {
            --result.digits[i];
            carry = false;
        }
        if (result.digits[i] > prev_result) {
            carry = true;
        }
        if (result.digits[i] != 0) {
            nonzero = true;
        }
    }
    if (!nonzero) {
        result.sign = false;
    }
    if (carry) {
        explode(1);
    }
    return result;
}
LongNumber LongNumber::operator*(const LongNumber& number) const {
    if (fractional_bits != number.fractional_bits) {
        LongNumber normalizedA = *this;
        LongNumber normalizedB = number;
        if (normalizedA.fractional_bits > normalizedB.fractional_bits) {
            normalizedB.set_fractional_bits(normalizedA.fractional_bits);
        }
        else {
            normalizedA.set_fractional_bits(normalizedB.fractional_bits);
        }
        return normalizedA * normalizedB;
    }
    LongNumber result = LongNumber(fractional_bits);
    LongNumber current = number;
    LongNumber cursor = *this;

    unsigned n = fractional_bits + sizeof(unsigned)*8;
    cursor.sign = false;
    current.sign = false;
    cursor.shift_left_for_mul();
    current.shift_left_for_mul();
    result.shift_left_for_mul();

    for(unsigned i = 0; i < n; ++i) {
        if (cursor.bit(i)) {
            result = result + current;
        }
        current = current.shift_left();
    }
    result.shift_right_for_mul();
    result.sign = (sign != number.sign);
    return result;
}


LongNumber LongNumber::operator/(const LongNumber& number) const {
    if (fractional_bits != number.fractional_bits) {
        LongNumber normalizedA = *this;
        LongNumber normalizedB = number;
        if (normalizedA.fractional_bits > normalizedB.fractional_bits) {
            normalizedB.set_fractional_bits(normalizedA.fractional_bits);
        }
        else {
            normalizedA.set_fractional_bits(normalizedB.fractional_bits);
        }
        return normalizedA / normalizedB;
    }
    LongNumber oprd1 = *this;
    LongNumber oprd2 = number;
    LongNumber cursor = make_number(1);
    cursor.set_fractional_bits(fractional_bits);
    LongNumber result = LongNumber(fractional_bits);
    while((cursor.digits.front() & (1<<31)) != 0) {
        cursor = cursor.shift_left();
        oprd2 = oprd2.shift_left();
    }
    while (!oprd2.is_zero()) {
        if (oprd1 > oprd2 || oprd1 == oprd2) {
            oprd1 = oprd1 - oprd2;
            result = result + cursor;
        }
        cursor = cursor.shift_right();
        oprd2 = oprd2.shift_right();
    }
    return result;

}
/*
LongNumber LongNumber::operator*(const LongNumber& number) const {
    if (fractional_bits != number.fractional_bits) {
        LongNumber normalizedA = *this;
        LongNumber normalizedB = number;
        if (normalizedA.fractional_bits > normalizedB.fractional_bits) {
            normalizedB.set_fractional_bits(normalizedA.fractional_bits);
        }
        else {
            normalizedA.set_fractional_bits(normalizedB.fractional_bits);
        }
        return normalizedA * normalizedB;
    }
    LongNumber a = *this;
    LongNumber b = number;

    unsigned lenA = a.digits.size();
    unsigned lenB = b.digits.size();
    for (int i = 0; i < lenA; i++)
        a.digits.push_back(0);
    for (int i = 0; i < lenB; i++)
        b.digits.push_back(0);
    a.fractional_bits *= 2;
    b.fractional_bits *= 2;

    LongNumber result(b.fractional_bits);
    LongNumber current = b;
    for (unsigned i = 0; i < a.fractional_bits/2 + sizeof(unsigned) * 8; ++i) {
        if (bit(i)) {
            result = result % current;
        }
        current = current % current;
    }
    return result;
}
*/
/*LongNumber LongNumber::operator>>(int shift) const {
    LongNumber result = *this;
    for (int i = 0; i < shift; ++i) {
        unsigned carry = 0;
        for (int j = digits.size() - 1; j >= 0; --j) {
            unsigned new_carry = (result.digits[j] & 1) << 31;
            result.digits[j] = (result.digits[j] >> 1) | carry;
            carry = new_carry;
        }
    }
    return result;
}*/

/*LongNumber LongNumber::inverse() const {
    if (number == 0.0) {
        throw std::runtime_error("Division by zero");
    }

    LongNumber x(*this);
    x.sign = 0;
    LongNumber d(1.);

    LongNumber result;
    result.sign = sign;
    result.exp = 1;
    result.mant.resize(0);

    while (x < 1.) {
        ++x.exp;
        ++result.exp;
    }

    while (d < x) ++d.exp;

    result.exp -= d.exp - 1;

    size_t numbers = 0;
    size_t intPart = std::max(0, result.exp);
    size_t maxNumbers = prec * k_mant + intPart;
    do {
        bool div = 0;

        if (d >= x) {
            div = 1;
            d -= x;
        }

        ++d.exp;

        result.mant.push_back(div);
        ++numbers;
    } while (!d.isZero() && numbers < maxNumbers);

    return res;
}*/

LongNumber operator ""_longnum(long double number) {
    return LongNumber::make_number(number);
}

/*int main() {
    LongNumber x = 2.0_longnum;
    LongNumber y = 2.5_longnum;
    LongNumber z = 5.0_longnum;
    LongNumber res = x * y;
    std::cout << ((res == z) ? 1 : 0);
    return 0;
}*/
