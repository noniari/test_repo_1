#include "egyptian_fractions.h"

#include <iostream>
#include <stdexcept>

int main() {
    unsigned int numerator;
    unsigned int denominator;
    if (!(std::cin >> numerator >> denominator)) {
        std::cerr << "Expected: numerator denominator.\n";
        return 1;
    }
    try {
        const denom_array_t result = egyptian_fractions(numerator, denominator);
        for (std::size_t index = 0; index < result.sz; ++index) {
            if (index != 0) {
                std::cout << " + ";
            }
            std::cout << "1/" << result.arr[index];
        }
        std::cout << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}