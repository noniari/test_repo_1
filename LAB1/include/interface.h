#include <common.h>
#include <game8.h>

void print_state(const State& state);

State parse_state(const std::string& input);

void write_solution(const std::string& file_name,
	const std::vector<State>& solution);