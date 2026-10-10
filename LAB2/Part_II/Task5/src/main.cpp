#include "cover_points.h"

#include <iostream>
#include <vector>

int main() {
    int count;
    if (!(std::cin >> count) || count < 0) {
        std::cerr << "Expected: n, then n point coordinates, then unitlen.\n";
        return 1;
    }
    std::vector<int> points(static_cast<std::size_t>(count));
    for (int& point : points) {
        if (!(std::cin >> point)) {
            std::cerr << "Incomplete point list.\n";
            return 1;
        }
    }
    int unitLength;
    if (!(std::cin >> unitLength)) {
        std::cerr << "Missing segment length.\n";
        return 1;
    }
    try {
        std::cout << coverpoints(points, unitLength) << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}