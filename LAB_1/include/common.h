#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using State = std::vector<int>;

struct SearchResult {
	std::vector<State> path;
	std::size_t expanded = 0;
	double milliseconds = 0.0;
};