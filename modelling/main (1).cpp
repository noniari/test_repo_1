#include "normal_distribution.h"
#include <iostream>

int main() {
    NormalGenerator ng{10, 1};
    for (int i = 0; i < 100; ++i)
        std::cout << ng.generate() << std::endl;
    return 0;
}