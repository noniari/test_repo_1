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
}
