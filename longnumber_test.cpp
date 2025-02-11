#include <iostream>
#include <vector>
#include <algorithm>

#define explode exit

class LongNumber {
private:
    std::vector<unsigned> digits;
    unsigned fractional_bits;
    bool sign = false;
    static unsigned divide_up(unsigned a, unsigned b) {
        return (a % b == 0) ? a / b : a / b + 1;
    };
    bool bit(unsigned n) const {
        return (digits[n / 32] & (1 << (n % 32))) != 0;
    }

    LongNumber operator%(const LongNumber& number) const {
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

public:

    static LongNumber make_number(long double number) {
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
    LongNumber() {
        fractional_bits = 0;
        sign = 0;
        digits = {};
    }
    LongNumber(unsigned fractional_bits) {
        digits.assign(divide_up(fractional_bits, sizeof(unsigned) * 8) + 1, 0);
        this->fractional_bits = fractional_bits;
        this->sign = false;
    };

    LongNumber(const LongNumber& number) {
        this->digits = number.digits;
        this->fractional_bits = number.fractional_bits;
        this->sign = number.sign;
    };

    void set_fractional_bits(unsigned b) {
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

    /*
    LongNumber operator ""_longnum(long double number) {
        return make_number(number);
    }
    */
    LongNumber& operator=(const LongNumber& number) {
        this->digits = number.digits;
        this->fractional_bits = number.fractional_bits;
        this->sign = number.sign;
        return *this;
    }

    bool operator==(const LongNumber& number) {
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

    bool operator!=(const LongNumber& number) {
        return !(*this == number);
    }

    bool operator>(const LongNumber& number) {
        if (*this == number) {
            return false;
        }
        return !((*this - number).sign);
    }

    bool operator<(const LongNumber& number) {
        if (*this == number) {
            return false;
        }
        return (*this - number).sign;
    }

    LongNumber operator-() const {
        LongNumber result = *this;
        result.sign = !result.sign;
        return result;
    }

    LongNumber operator+(const LongNumber& number) const {
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


    LongNumber operator-(const LongNumber& number) const {
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

    LongNumber operator*(const LongNumber& number) const {
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

        unsigned lenA = a.digits.size() - 1;
        unsigned lenB = b.digits.size() - 1;
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

    ~LongNumber() {};
};

int main() {
    LongNumber x = LongNumber::make_number(2.0);
    LongNumber y = LongNumber::make_number(2.0);
    LongNumber z = LongNumber::make_number(4.0);
    LongNumber r(x + y);
    std::cout << (r == z) ? 1 : 0;
    //std::cout << "hello";
    return 0;
}

