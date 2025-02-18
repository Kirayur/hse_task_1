#include <iostream>
#include <cassert>
#include "second_version_pr1.cpp"

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
    LongNumber number1 = 13.533_longnum;
    LongNumber number2 = 1.83_longnum;
    LongNumber expected = 11.703_longnum; 
    assert(number1 - number2 == expected);
    std::cout << "Subtraction test: OK" << std::endl;
}

void multiplication() {
    LongNumber number1 = 2.0_longnum;
    LongNumber number2 = 10.5_longnum;
    LongNumber expected = 21.0_longnum;
    assert(number1 * number2 == expected);
    std::cout << "Multiplication test: OK" << std::endl;
}

void division() {
    LongNumber number1 = 5.0_longnum;
    LongNumber number2 = 0.1_longnum;
    LongNumber expected = 50.0_longnum;
    assert(number1 / number2 == expected);
    std::cout << "Division test: OK" << std::endl;
}

int main() {
    comparison();
    addition();
    subtraction();
    multiplication();
    division();
    std::cout << "All tests passed! (ура)" << std::endl;
    return 0;
}
