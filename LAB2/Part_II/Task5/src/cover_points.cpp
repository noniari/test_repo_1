#include "cover_points.h"

#include <algorithm>
#include <stdexcept>

int coverpoints(std::vector<int>& pts, int unitlen) {
    if (unitlen <= 0) {
        throw std::invalid_argument("The segment length must be positive.");
    }
    if (pts.empty()) {
        return 0;
    }

    std::sort(pts.begin(), pts.end());
    int count = 0;
    std::size_t index = 0;
    while (index < pts.size()) {
        const long long rightEnd = static_cast<long long>(pts[index]) + unitlen;
        ++count;
        ++index;
        while (index < pts.size() && pts[index] <= rightEnd) {
            ++index;
        }
    }
    return count;
}