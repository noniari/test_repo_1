#include "schedule.h"

#include <algorithm>
#include <stdexcept>

int schedulemax(std::vector<intvl_t>& reqs) {
    for (const intvl_t& request : reqs) {
        if (request.fin < request.start) {
            throw std::invalid_argument("An interval cannot finish before it starts.");
        }
    }
    std::sort(reqs.begin(), reqs.end(), [](const intvl_t& first, const intvl_t& second) {
        if (first.fin != second.fin) {
            return first.fin < second.fin;
        }
        if (first.start != second.start) {
            return first.start < second.start;
        }
        return first.number < second.number;
    });

    int count = 0;
    int lastFinish = 0;
    bool hasSelected = false;
    for (const intvl_t& request : reqs) {
        if (!hasSelected || request.start >= lastFinish) {
            ++count;
            lastFinish = request.fin;
            hasSelected = true;
        }
    }
    return count;
}