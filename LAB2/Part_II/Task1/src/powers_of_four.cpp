#include "powers_of_four.h"

#include <vector>

unsigned int powers_of_four(unsigned int n, unsigned int m) {
    if (n == 0) {
        return 0;
    }

    std::vector<unsigned int> coins{1};
    for (unsigned int exponent = 1; exponent <= m; ++exponent) {
        const unsigned int previous = coins.back();
        if (previous > n / 4) {
            break;
        }
        coins.push_back(previous * 4);
    }

    unsigned int count = 0;
    for (auto coin = coins.rbegin(); coin != coins.rend(); ++coin) {
        count += n / *coin;
        n %= *coin;
    }
    return count;
}