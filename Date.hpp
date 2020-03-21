#pragma once

#include <date/date.h>

#include "TimeDelta.hpp"

namespace datetime {
class Date {
public:
	static Date Today() {
		return {date::floor<date::days>(std::chrono::system_clock::now())};
	}

	template<class Rep>
	static Date FromTimestamp(Rep timestamp) {
		//system_clock::time_point dt{std::chrono::seconds{timestamp}}; // compatible with old compilers ?
		std::chrono::seconds dur(timestamp);
		std::chrono::time_point<std::chrono::system_clock> dt(dur);

		return {date::floor<date::days>(dt)}; // convert time_point to sys_days
	}

	Date() = default;
	Date(const date::year_month_day &ymd) :
		_ymd(ymd) {
	}
	Date(const date::year &y, const date::month &m, const date::day &d) :
		_ymd(y, m, d) {
	}

	const date::year_month_day &YearMonthDay() const { return _ymd; }
	
	date::year Year() const { return _ymd.year(); }
	date::month Month() const { return _ymd.month(); }
	date::day Day() const { return _ymd.day(); }

	date::weekday ObjWeekday() const {
		return date::weekday(_ymd);
	}
	unsigned Weekday() const {
		return (ObjWeekday().c_encoding() - 1) % 7; // can do better
	}
	unsigned IsoWeekday() const {
		return 1 + Weekday();
	}
	
	std::string Format(std::string_view format) const {
		return date::format(format.data(), _ymd);
	}
private:
	date::year_month_day _ymd;
};

inline Date operator+(const Date &d, const TimeDelta &td) {
	return {date::sys_days(d.YearMonthDay()) + date::days(td.Days())};
}

inline Date operator+(const TimeDelta &td, const Date &d) {
	return d + td;
}

inline Date operator-(const Date &d, const TimeDelta &td) {
	return {date::sys_days(d.YearMonthDay()) - date::days(td.Days())};
}

inline TimeDelta operator-(const Date &x, const Date &y) {
	return {date::sys_days(x.YearMonthDay()) - date::sys_days(y.YearMonthDay())};
}

inline bool operator==(const Date &x, const Date &y) {
	return x.YearMonthDay() == y.YearMonthDay();
}

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os, const Date &date) {
	return os << date.YearMonthDay();
}
}
