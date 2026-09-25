#include "common.h"

struct StateHash {
    size_t operator()(const State& state) const;
};

std::vector<State> reconstruct_path(
        const std::unordered_map<State, State, StateHash>& parent
      , State current);

SearchResult BFS_solve(const State& init_state);
SearchResult DLS_solve(const State& init_state, int limit);
SearchResult A_star_solve(const State& init_state, const std::function<int(const State&)>& heuristic);
int misplaced_tiles(const State& state);
int manhattan_distance(const State& state);