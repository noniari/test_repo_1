#include <ai.h>
#include <interface.h>

int main() {
    std::string input =
        "2 1 0 0 0 0 0 "
        "0 1 0 1 1 1 0 "
        "0 0 0 0 0 1 0 "
        "1 1 1 1 0 1 0 "
        "0 0 0 1 0 0 0 "
        "0 1 0 1 1 1 0 "
        "0 0 0 0 0 0 0";
    State start = parse_state(input);
    auto solution = BFS_solve(start);
    auto dls_solution = DLS_solve(start, 100);
    auto heuristic_solution = EST_solve(start, manhattan_distance);
    auto second_heuristic_solution = EST_solve(start, direct_distance);

    if (solution.first.empty()) std::cout << "Solution was not found" << std::endl;
    else for (auto state : solution.first) print_state(state);
    std::cout << "Solution has been found by "
              << (solution.first.empty() ? 0 : solution.first.size() - 1)
              << " turns" << std::endl;
    std::cout << "BFS visited " << solution.second << " states" << std::endl;
    std::cout << "DLS visited " << dls_solution.second << " states" << std::endl;
    std::cout << "A* Manhattan visited " << heuristic_solution.second << " states" << std::endl;
    std::cout << "A* direct heuristic visited " << second_heuristic_solution.second << " states" << std::endl;
    write_solution("sol(Var2).txt", solution.first);
    
    return 0;
}