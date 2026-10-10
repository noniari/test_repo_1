#include <ai.h>
#include <interface.h>
#include <chrono>
#include <fstream>
#include <iomanip>

struct RunResult {
    std::string start;
    std::string algorithm;
    int path_length;
    int visited;
    double elapsed_microseconds;
    std::vector<State> solution;
};

int main() {
    const std::string input =
        "1 1 1 1 1 1 1 "
        "1 1 0 1 0 0 1 "
        "1 0 0 0 0 1 1 "
        "1 1 0 1 0 1 1 "
        "1 0 0 1 0 0 1 "
        "1 1 0 0 1 0 1 "
        "1 1 1 1 1 0 1";
    const std::vector<std::pair<std::string, int>> starts = {
        {"Start 1 (1,2)", 9},
        {"Start 2 (2,2)", 16},
        {"Start 3 (3,2)", 23},
        {"Start 4 (4,4)", 32},
        {"Start 5 (4,5)", 33}
    };
    const int depth_limit = MAZE_SIZE * MAZE_SIZE - 1;
    std::vector<RunResult> results;

    auto measure = [&](const std::string& start_name,
                       const std::string& algorithm, auto solve) {
        auto started = std::chrono::high_resolution_clock::now();
        auto solution = solve();
        auto finished = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::micro>(
            finished - started).count();
        int path_length = solution.first.empty()
            ? -1 : static_cast<int>(solution.first.size()) - 1;
        results.push_back({start_name, algorithm, path_length,
                           solution.second, elapsed, solution.first});
    };

    for (size_t index = 0; index < starts.size(); ++index) {
        State start = parse_state(input);
        for (int& cell : start)
            if (cell == PLAYER) cell = EMPTY;
        start[starts[index].second] = PLAYER;
        measure(starts[index].first, "BFS", [&]() {
            return BFS_solve(start);
        });
        measure(starts[index].first, "DLS", [&]() {
            return DLS_solve(start, depth_limit);
        });
        measure(starts[index].first, "A* Manhattan", [&]() {
            return EST_solve(start, manhattan_distance);
        });
        measure(starts[index].first, "A* Euclidean", [&]() {
            return EST_solve(start, euclidean_distance);
        });
    }

    auto write_table = [](std::ostream& output,
                          const std::vector<RunResult>& rows) {
        output << "| Start Position | Algorithm | Path Length (moves) | Expanded Vertices | Time (us) |\n"
               << "|---|---|---:|---:|---:|\n";
        for (const auto& row : rows) {
            output << "| " << row.start << " | " << row.algorithm << " | ";
            if (row.path_length < 0) output << "not found";
            else output << row.path_length;
            output << " | " << row.visited << " | "
                   << std::fixed << std::setprecision(2)
                   << row.elapsed_microseconds << " |\n";
        }
    };

        std::ofstream report("sol(Var2).txt");
        for (const auto& row : results) {
            report << row.start << " | " << row.algorithm << '\n';
            if (row.solution.empty()) {
                report << "Solution not found\n\n";
                continue;
            }

            for (const auto& state : row.solution) {
                for (int cell = 0; cell < static_cast<int>(state.size()); ++cell) {
                    if (cell % MAZE_SIZE != 0) report << ' ';
                    report << state[cell];
                    if ((cell + 1) % MAZE_SIZE == 0) report << '\n';
                }
                report << '\n';
            }
            report << "Path length: " << row.path_length << " moves\n\n";
        }
        report << "Results\n";
    write_table(std::cout, results);
    write_table(report, results);

    return 0;
}