#pragma once

#include <vector>

struct intvl_t {
    int number;
    int start;
    int fin;
};

int schedulemax(std::vector<intvl_t>& reqs);