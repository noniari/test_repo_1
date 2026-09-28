#include <common.h>

struct StateHash {
    size_t operator()(const State& state) const;
};

std::vector<State> reconstruct_path(
        const std::unordered_map<State, State, StateHash>& parent
      , State current);

std::pair<std::vector<State>, int> BFS_solve(State init_state);

bool dls(const State& state, int depth, int limit, 
        std::vector<State>& solution,
        std::unordered_set<State, StateHash>& closed,
        std::unordered_map<State, int, StateHash>& shallowest_depth,
        int& current_visited);

std::pair<std::vector<State>, int> DLS_solve(State init_state, int limit);

int manhattan_distance(const State& state);
int euclidean_distance(const State& state);

std::pair<std::vector<State>, int> EST_solve(State init_state, int (*est)(const State&));