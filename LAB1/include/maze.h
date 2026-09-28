#include <common.h>

constexpr int MAZE_SIZE = 7;
constexpr int WALL = 1;
constexpr int EMPTY = 0;
constexpr int PLAYER = 2;

bool is_goal(const State& state);

int find_player(const State& state);

std::vector<int> get_possible_moves(const State& state, int player_index);

State make_move(const State& state, int player_index, int target_index);


