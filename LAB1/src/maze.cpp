#include <maze.h>

const int GOAL_INDEX = 48;

bool is_goal(const State &state) { return find_player(state) == GOAL_INDEX; }

int find_player(const State &state) {
    for (int i = 0; i < static_cast<int>(state.size()); ++i)
        if (state[i] == PLAYER) return i;
    return -1;
}

std::vector<int> get_possible_moves(const State& state, int player_index) {
    std::vector<int> moves{};
    int row = player_index / MAZE_SIZE;
    int col = player_index % MAZE_SIZE;

    if (row > 0 && state[player_index - MAZE_SIZE] != WALL)
        moves.push_back(player_index - MAZE_SIZE);
    if (row + 1 < MAZE_SIZE && state[player_index + MAZE_SIZE] != WALL)
        moves.push_back(player_index + MAZE_SIZE);
    if (col > 0 && state[player_index - 1] != WALL)
        moves.push_back(player_index - 1);
    if (col + 1 < MAZE_SIZE && state[player_index + 1] != WALL)
        moves.push_back(player_index + 1);
    return moves;
}

State make_move(const State &state, int player_index, int target_index) {
    State new_state = state;
    if (new_state[target_index] == WALL) return state;
    new_state[player_index] = EMPTY;
    new_state[target_index] = PLAYER;
    return new_state;
}
