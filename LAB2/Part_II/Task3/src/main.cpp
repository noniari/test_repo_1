#include "schedule.h"

#include <iostream>
#include <vector>

int main() {
    int count;
    if (!(std::cin >> count) || count < 0) {
        std::cerr << "Expected a non-negative request count followed by number/start/finish triples.\n";
        return 1;
    }
    std::vector<intvl_t> requests(static_cast<std::size_t>(count));
    for (intvl_t& request : requests) {
        if (!(std::cin >> request.number >> request.start >> request.fin)) {
            std::cerr << "Incomplete request list.\n";
            return 1;
        }
    }
    try {
        std::cout << schedulemax(requests) << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}