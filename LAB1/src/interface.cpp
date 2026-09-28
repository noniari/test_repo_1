#include <interface.h>
#include <fstream>

void print_state(const State &state) {
    for (int i = 0; i < static_cast<int>(state.size()); ++i) {
        std::cout << state[i] << " ";
        if ((i + 1) % MAZE_SIZE == 0) std::cout << std::endl;
    }
    std::cout << std::endl; return;
}

State parse_state(const std::string &input) {
    State state;
    std::stringstream ss(input);
    int num;
    while (ss >> num) state.push_back(num);
    return state;
}

void write_solution(const std::string& file_name,
        const std::vector<State>& solution) {
    std::ofstream output(file_name);
    for (const auto& state : solution) {
        for (int i = 0; i < static_cast<int>(state.size()); ++i) {
            if (i % MAZE_SIZE != 0) output << " ";
            output << state[i];
            if ((i + 1) % MAZE_SIZE == 0) output << '\n';
        }
        output << '\n';
    }
    output << "Solution has been found by "
           << (solution.empty() ? 0 : solution.size() - 1) << " turns\n";
}
