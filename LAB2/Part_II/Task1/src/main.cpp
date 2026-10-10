#include "powers_of_four.h"

#include <iostream>

int main() {
    unsigned int number;
    unsigned int maxExponent;
    if (!(std::cin >> number >> maxExponent)) {
        std::cerr << "Expected: n m (m is the largest allowed exponent).\n";
        return 1;
    }
    std::cout << powers_of_four(number, maxExponent) << '\n';
    return 0;
}