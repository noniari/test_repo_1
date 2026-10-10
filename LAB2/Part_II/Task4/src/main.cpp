#include "cover_intervals.h"

#include <iostream>
#include <vector>

int main() {
    int left;
    int right;
    int count;
    if (!(std::cin >> left >> right >> count) || count < 0) {
        std::cerr << "Expected: L R n followed by n interval endpoint pairs.\n";
        return 1;
    }
    std::vector<intvl_t> intervals(static_cast<std::size_t>(count));
    for (intvl_t& interval : intervals) {
        if (!(std::cin >> interval.start >> interval.end)) {
            std::cerr << "Incomplete interval list.\n";
            return 1;
        }
    }
    try {
        const int answer = covermin(left, right, intervals);
        if (answer < 0) {
            std::cout << "Impossible\n";
        } else {
            std::cout << answer << '\n';
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}