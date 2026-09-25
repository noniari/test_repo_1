#include "ai.h"
#include "interface.h"

#include <iomanip>

void print_result(const std::string& name, const SearchResult& result, int limit, bool optimal) {
    const bool found = !result.path.empty();
    const bool reasonable = found && result.expanded < 10000;
    std::cout << std::left << std::setw(16) << name
              << std::setw(9) << result.expanded
              << std::setw(10) << std::fixed << std::setprecision(3) << result.milliseconds
              << std::setw(7) << (found ? static_cast<int>(result.path.size()) - 1 : -1)
              << std::setw(6) << (limit >= 0 ? std::to_string(limit) : "-")
              << std::setw(7) << (found ? "yes" : "no")
              << std::setw(8) << (found && optimal ? "yes" : "no")
              << (reasonable ? "reasonable" : "expensive") << '\n';
}

int main() {
    const std::vector<std::string> inputs = {
        "1 2 3 4 5 6 7 8 9 10 11 12 13 14 0 15",
        "1 2 3 4 5 6 7 8 9 10 11 12 13 0 14 15",
        "1 2 3 4 5 6 7 8 9 10 11 0 13 14 15 12",
        "1 2 3 4 5 6 7 8 9 10 0 12 13 14 11 15",
        "1 2 3 4 5 6 7 8 9 0 11 12 13 10 14 15"
    };

    std::cout << "15-puzzle: BFS, DLS and A*\n";
    std::cout << "expanded    - number of expanded states\n";
    std::cout << "time_ms     - execution time in milliseconds\n\n";
    for (std::size_t index = 0; index < inputs.size(); ++index) {
        const State start = parse_state(inputs[index]);
        const SearchResult bfs = BFS_solve(start);
        const SearchResult dls = DLS_solve(start, 6);
        const SearchResult misplaced = A_star_solve(start, misplaced_tiles);
        const SearchResult manhattan = A_star_solve(start, manhattan_distance);
        const int optimal_length = bfs.path.empty() ? -1 : static_cast<int>(bfs.path.size()) - 1;

        std::cout << "\nStart " << index + 1 << ": " << inputs[index] << '\n';
        std::cout << std::left << std::setw(16) << "algorithm"
              << std::setw(9) << "expanded" << std::setw(10) << "time_ms"
              << std::setw(7) << "length" << std::setw(6) << "limit"
              << std::setw(7) << "found" << std::setw(8) << "optimal" << "purpose\n";
        print_result("BFS", bfs, -1, true);
        print_result("DLS", dls, 6, !dls.path.empty() && static_cast<int>(dls.path.size()) - 1 == optimal_length);
        print_result("A* misplaced", misplaced, -1,
                     !misplaced.path.empty() && static_cast<int>(misplaced.path.size()) - 1 == optimal_length);
        print_result("A* Manhattan", manhattan, -1,
                     !manhattan.path.empty() && static_cast<int>(manhattan.path.size()) - 1 == optimal_length);
    }

    return 0;
}