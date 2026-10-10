#pragma once

#include <cstddef>
#include <vector>

struct denom_array_t {
    std::vector<unsigned int> arr;
    std::size_t sz = 0;
};

denom_array_t egyptian_fractions(unsigned int num, unsigned int den);