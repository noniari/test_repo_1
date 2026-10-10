#include "funparty.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

answer_t funparty(kid_t* kids, int nkids) {
    if (nkids < 0 || (nkids > 0 && kids == nullptr)) {
        throw std::invalid_argument("Invalid child array or count.");
    }

    if (nkids == 0) {
        return {0, nullptr};
    }

    std::vector<int> order(static_cast<std::size_t>(nkids));
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [kids](int first, int second) {
        if (kids[first].age != kids[second].age) {
            return kids[first].age < kids[second].age;
        }
        return kids[first].id < kids[second].id;
    });

    answer_t answer{0, new kidgroup_t[static_cast<std::size_t>(nkids)]};
    int group = -1;
    int youngestAge = 0;
    for (int index : order) {
        if (group == -1 || static_cast<long long>(kids[index].age) - youngestAge > 2) {
            ++group;
            youngestAge = kids[index].age;
        }
        answer.mapping[index] = {kids[index].id, group};
    }
    answer.ngroups = group + 1;
    return answer;
}