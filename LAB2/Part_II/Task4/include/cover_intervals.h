#pragma once

#include <vector>

struct intvl_t {
    int start;
    int end;
};

int covermin(int L, int R, std::vector<intvl_t>& intervs);