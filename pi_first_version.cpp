#include <vector>
#include <iostream>
#include "rand12.cpp"
#include <ctime>

#define precision 512


LongNumber pi(int n) {
    LongNumber one = 1.0_longnum;
    one.set_fractional_bits(precision);
    LongNumber two = 2.0_longnum;
    two.set_fractional_bits(precision);
    LongNumber four = 4.0_longnum;
    four.set_fractional_bits(precision);
    LongNumber five = 5.0_longnum;
    five.set_fractional_bits(precision);
    LongNumber six = 6.0_longnum;
    six.set_fractional_bits(precision);
    LongNumber sixteen = 16.0_longnum;
    sixteen.set_fractional_bits(precision);
    LongNumber multiplier = 1.0_longnum;
    multiplier.set_fractional_bits(precision);
    LongNumber answer = 0.0_longnum;
    answer.set_fractional_bits(precision);
    for (int i = 0; i < n; ++i) {
        LongNumber curr_i = LongNumber::make_number(i);
        curr_i = curr_i + curr_i;
        curr_i = curr_i + curr_i;
        curr_i = curr_i + curr_i;
        answer = answer + multiplier * (four/(curr_i+one) - two/(curr_i+four) - one/(curr_i + five) - one/(curr_i+ six));
        multiplier = multiplier / sixteen;
    }
    return answer;
}

int main(int argc, char** argv) {
    int prec = (argc < 2) ? 100 : std::stoi(std::string(argv[1]));
    clock_t start = clock();
    LongNumber pi_computed = pi(prec * 2);
    double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << pi_computed.digits.back() << ".";
    for (int i = 0; i < prec; i++) {
        pi_computed = (pi_computed - LongNumber::make_number(pi_computed.digits.back())) * 10.0_longnum;
        std::cout << pi_computed.digits.back();
    }
    std::cout << "\nIt took: " << (int)(duration * 1000) << " ms\n";
    return 0;
}