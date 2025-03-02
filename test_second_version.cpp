#include <iostream>
#include <cassert>
#include "rand12.cpp"

void comparison() {
    LongNumber number1 = 2.60_longnum;
    LongNumber number2 = 2.6_longnum;
    LongNumber number3 = 1.3_longnum;
    assert(number1 == number2);
    std::cout << "Comparison test: OK" << std::endl;
    assert(number1 != number3);
    std::cout << "Comparison test: OK" << std::endl;
    assert(number1 > number3);
    std::cout << "Comparison test: OK" << std::endl;
    assert(number3 < number1);
    std::cout << "Comparison test: OK" << std::endl;
}

void addition() {
    LongNumber number1 = 13.5332_longnum;
    LongNumber number2 = 1.837_longnum;
    LongNumber expected = 15.3702_longnum; 
    assert(number1 + number2 == expected);
    std::cout << "Addition test: OK" << std::endl;
}

void subtraction() {
    LongNumber number1 = 1.0_longnum;
    LongNumber number2 = 2.0_longnum;
    LongNumber expected = -1.0_longnum; 
    assert(number1 - number2 == expected);
    std::cout << "Subtraction test: OK" << std::endl;
}

bool equal_eps(LongNumber a, LongNumber b) {
    return (a-b).abs() < 0.000001_longnum;
}

void multiplication() {
    LongNumber number1 = 2.0_longnum;
    LongNumber number2 = 10.05_longnum;
    LongNumber expected = 20.1_longnum;
    assert(equal_eps(number1 * number2, expected));
    std::cout << "Multiplication test: OK" << std::endl;
}

void division() {
    LongNumber number1 = 12.12_longnum;
    LongNumber number2 = 5.0_longnum;
    LongNumber expected = 2.424_longnum;
    assert(equal_eps(number1 / number2, expected));
    std::cout << "Division test: OK" << std::endl;
}

int main() {
    comparison();
    addition();
    subtraction();
    multiplication();
    division();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
