#include "second_version_header.hpp"
#include <stdexcept>

unsigned LongNumber::divide_up(unsigned a, unsigned b) {
    return (a % b == 0) ? a / b : a / b + 1;
}

bool LongNumber::bit(unsigned n) const {
    return (digits[n / 32] & (1 << (n % 32))) != 0;
}

LongNumber LongNumber::operator%(const LongNumber& number) const {
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
        return normalizedA % normalizedB;
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
    return result;
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

bool LongNumber::operator==(const LongNumber& number) {
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
    return (this->sign == number.sign && this->digits == number.digits);
}

bool LongNumber::operator!=(const LongNumber& number) {
    return !(*this == number);
}

bool LongNumber::operator>(const LongNumber& number) {
    if (*this == number) {
        return false;
    }
    return !((*this - number).sign);
}

bool LongNumber::operator<(const LongNumber& number) {
    if (*this == number) {
        return false;
    }
    return (*this - number).sign;
}

LongNumber LongNumber::operator-() const {
    LongNumber result = *this;
    result.sign = !result.sign;
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

LongNumber LongNumber::operator/(const LongNumber& number) const {
    /*if (number == 0.0) {
        throw std::runtime_error("Division by zero");
    }*/

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

    LongNumber dividend = *this;
    LongNumber divisor = number;

    bool result_sign = (dividend.sign != divisor.sign);
    dividend.sign = false;
    divisor.sign = false;

    LongNumber result(dividend.fractional_bits);
    result.sign = result_sign;

    LongNumber shifted_divisor = divisor;
    LongNumber remainder = dividend;
    int shift_count = 0;

    while (shifted_divisor < remainder || shifted_divisor < remainder) {
        shifted_divisor = shifted_divisor + shifted_divisor; // Умножаем на 2
        shift_count++;
    }

    for (int i = shift_count - 1; i >= 0; --i) {
        shifted_divisor = shifted_divisor >> 1; // Сдвиг вправо (деление на 2)
        if (shifted_divisor < remainder || shifted_divisor == remainder) {
            remainder = remainder - shifted_divisor;
            result.digits[i / 32] |= (1 << (i % 32)); // Установка бита
        }
    }
    
    return result;
}

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
