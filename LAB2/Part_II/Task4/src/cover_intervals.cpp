#include "cover_intervals.h"

#include <algorithm>
#include <stdexcept>

int covermin(int L, int R, std::vector<intvl_t>& intervs) {
    if (L > R) {
        throw std::invalid_argument("The left boundary must not exceed the right boundary.");
    }
    for (const intvl_t& interval : intervs) {
        if (interval.end < interval.start) {
            throw std::invalid_argument("An interval cannot end before it starts.");
        }
    }
    if (L == R) {
        return 0;
    }

    std::sort(intervs.begin(), intervs.end(), [](const intvl_t& first, const intvl_t& second) {
        if (first.start != second.start) {
            return first.start < second.start;
        }
        return first.end > second.end;
    });

    int coveredUntil = L;
    std::size_t index = 0;
    int selected = 0;
    while (coveredUntil < R) {
        int farthest = coveredUntil;
        while (index < intervs.size() && intervs[index].start <= coveredUntil) {
            farthest = std::max(farthest, intervs[index].end);
            ++index;
        }
        if (farthest == coveredUntil) {
            return -1;
        }
        coveredUntil = farthest;
        ++selected;
    }
    return selected;
}