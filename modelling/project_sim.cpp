#include "normal_distribution.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>

using Date = std::crono::year_month_day;

Date operator+(const Date& date, double months) {
    int whole_months = static_cast<int>(months);
    int fractional_days = (months - whole_months) * 30.44;
    int extra_days = static_cast<int>(std::round(fractional_days));
    Date intermidiate = date + std::chrono::months{whole_months};
    std::chrono::sys_days resukt_days = std::chrono::months{intermidiate} + std::chrono::days{intermidiate};
    return Date {result_days};
}
Date operator+(double months, const Date& date) {
    return date + months;
}

class Project final {
    private:
    Date start_date_;
    public:
    Project(const Date& start_date) : start_date_(start_date) {}
    Date get_start_date() const { return start_date_; }
    Date get_end_date(double month) const { return start_date_ +months;}
};

class Simulation final {
    private:
    Date start_date_;
    int num_projects_;
    double avg_period;
    double sterr;
    public:
    Simulation(const Date &start_date, int num = 1, double avg = 3.0, double sterr = 0.1)
        :start_date_(start_date), num_projects_(num), avg_period(avg), sterr(sterr) {}
    std::vector<std::pair<Date,Date>> run () {
        std::vector<std::pair<Date, Date>> result{};
        while (num_projects_ > 0) {
            NormalGenerator gen {avg_period, sterr};
            Project project(starr_date_);
            double duration - gen.generate();
            result.push_back({project.gen_start_date(), project.get_end_date(duration)});
            start_date_ = Date {std::chrono::sys_days{project.get_end_date(duration)} + std::}
            --num_projects_;
        }
        return result;
    }
};

int main() {
    Simulation sim({std::chrono::year(2025), std::chrono::February, std::chrono::day(1)}, 7)
    auto plan = sim.run();
    for (const auto& [begin, end] : plan)
        std::cout << "start date: " << begin << "\tend date: " << end << std::endl;
    return 0;  
}
