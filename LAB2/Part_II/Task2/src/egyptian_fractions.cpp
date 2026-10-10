#include "egyptian_fractions.h"

#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>

denom_array_t egyptian_fractions(unsigned int num, unsigned int den) {
    if (den == 0) {
        throw std::invalid_argument("The denominator must be positive.");
    }

    denom_array_t result;
    if (num == 0) {
        return result;
    }

    const unsigned int divisor = std::gcd(num, den);
    std::uint64_t numerator = num / divisor;
    std::uint64_t denominator = den / divisor;
    while (numerator != 0) {
        const std::uint64_t unitDenominator =
            denominator / numerator + (denominator % numerator != 0);
        if (unitDenominator > std::numeric_limits<unsigned int>::max()) {
            throw std::overflow_error("An Egyptian fraction denominator exceeds unsigned int.");
        }
        result.arr.push_back(static_cast<unsigned int>(unitDenominator));

        const std::uint64_t remainder = denominator % numerator;
        if (remainder == 0) {
            break;
        }
        const std::uint64_t nextNumerator = numerator - remainder;
        if (denominator > std::numeric_limits<std::uint64_t>::max() / unitDenominator) {
            throw std::overflow_error("Intermediate Egyptian fraction value exceeds uint64_t.");
        }
        const std::uint64_t nextDenominator = denominator * unitDenominator;
        const std::uint64_t reduction = std::gcd(nextNumerator, nextDenominator);
        numerator = nextNumerator / reduction;
        denominator = nextDenominator / reduction;
    }
    result.sz = result.arr.size();
    return result;
}