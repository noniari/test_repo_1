#include "funparty.h"

#include <iostream>
#include <memory>
#include <vector>

int main() {
    int count;
    if (!(std::cin >> count) || count < 0) {
        std::cerr << "Expected: nkids followed by id/age pairs.\n";
        return 1;
    }
    std::vector<kid_t> kids(static_cast<std::size_t>(count));
    for (kid_t& kid : kids) {
        if (!(std::cin >> kid.id >> kid.age)) {
            std::cerr << "Incomplete child list.\n";
            return 1;
        }
    }
    try {
        const answer_t answer = funparty(kids.data(), count);
        std::unique_ptr<kidgroup_t[]> mapping(answer.mapping);
        std::cout << answer.ngroups << '\n';
        for (int index = 0; index < count; ++index) {
            std::cout << mapping[index].nkid << ' ' << mapping[index].ngroup << '\n';
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}