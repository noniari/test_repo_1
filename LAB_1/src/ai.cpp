#include "ai.h"
#include "game8.h"

#include <numeric>

namespace {

template <typename Solver>
SearchResult timed(Solver solver) {
    const auto started = std::chrono::steady_clock::now();
    SearchResult result = solver();
    const auto finished = std::chrono::steady_clock::now();
    result.milliseconds = std::chrono::duration<double, std::milli>(finished - started).count();
    return result;
}

}

size_t StateHash::operator()(const State& state) const {
    return std::accumulate(state.begin(), state.end(), size_t(0),
        [](size_t hash, int tile) { return hash * 17 + static_cast<size_t>(tile); });
}

std::vector<State> reconstruct_path(
        const std::unordered_map<State, State, StateHash>& parent, State current) {
    std::vector<State> path;
    while (true) {
        path.push_back(current);
        const auto it = parent.find(current);
        if (it == parent.end() || it->second == current) break;
        current = it->second;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

SearchResult BFS_solve(const State& init_state) {
    return timed([&]() {
        SearchResult result;
        std::queue<State> open;
        std::unordered_set<State, StateHash> visited;
        std::unordered_map<State, State, StateHash> parent;
        open.push(init_state);
        visited.insert(init_state);
        parent[init_state] = init_state;

        while (!open.empty()) {
            State current = open.front();
            open.pop();
            ++result.expanded;
            if (is_goal(current)) {
                result.path = reconstruct_path(parent, current);
                return result;
            }
            const int empty_index = find_empty(current);
            for (int target_index : get_possible_moves(empty_index)) {
                State next = make_move(current, empty_index, target_index);
                if (visited.insert(next).second) {
                    parent[next] = current;
                    open.push(std::move(next));
                }
            }
        }
        return result;
    });
}

SearchResult DLS_solve(const State& init_state, int limit) {
    return timed([&]() {
        SearchResult result;
        std::vector<State> path{init_state};
        std::unordered_set<State, StateHash> on_path{init_state};
        std::unordered_map<State, int, StateHash> best_depth;

        std::function<bool(const State&, int)> visit = [&](const State& current, int depth) {
            ++result.expanded;
            if (is_goal(current)) return true;
            if (depth == limit) return false;
            const auto known = best_depth.find(current);
            if (known != best_depth.end() && known->second <= depth) return false;
            best_depth[current] = depth;
            const int empty_index = find_empty(current);
            for (int target_index : get_possible_moves(empty_index)) {
                State next = make_move(current, empty_index, target_index);
                if (on_path.insert(next).second) {
                    path.push_back(next);
                    if (visit(next, depth + 1)) return true;
                    path.pop_back();
                    on_path.erase(next);
                }
            }
            return false;
        };

        if (visit(init_state, 0)) result.path = path;
        return result;
    });
}

struct QueueNode {
    int score;
    int cost;
    State state;
    bool operator<(const QueueNode& other) const {
        return score > other.score;
    }
};

SearchResult A_star_solve(const State& init_state,
                          const std::function<int(const State&)>& heuristic) {
    return timed([&]() {
        SearchResult result;
        std::priority_queue<QueueNode> open;
        std::unordered_map<State, int, StateHash> cost;
        std::unordered_map<State, State, StateHash> parent;
        open.push({heuristic(init_state), 0, init_state});
        cost[init_state] = 0;
        parent[init_state] = init_state;

        while (!open.empty()) {
            QueueNode current = open.top();
            open.pop();
            if (current.cost != cost[current.state]) continue;
            ++result.expanded;
            if (is_goal(current.state)) {
                result.path = reconstruct_path(parent, current.state);
                return result;
            }
            const int empty_index = find_empty(current.state);
            for (int target_index : get_possible_moves(empty_index)) {
                State next = make_move(current.state, empty_index, target_index);
                const int next_cost = current.cost + 1;
                auto known = cost.find(next);
                if (known == cost.end() || next_cost < known->second) {
                    cost[next] = next_cost;
                    parent[next] = current.state;
                    open.push({next_cost + heuristic(next), next_cost, std::move(next)});
                }
            }
        }
        return result;
    });
}

int misplaced_tiles(const State& state) {
    int result = 0;
    for (int index = 0; index < 16; ++index) {
        if (state[index] != 0 && state[index] != index + 1) ++result;
    }
    return result;
}

int manhattan_distance(const State& state) {
    int result = 0;
    for (int index = 0; index < 16; ++index) {
        const int tile = state[index];
        if (tile == 0) continue;
        const int target = tile - 1;
        result += std::abs(index / 4 - target / 4) + std::abs(index % 4 - target % 4);
    }
    return result;
}
