#pragma once

#include <iomanip>
#include <cmath>

#include "DateTime.hpp"

namespace datetime {
template<class Duration>
DateTime<typename DateTime<Duration>::CommonDuration> DateTime<Duration>::Today() {
	return date::make_zoned(date::current_zone(), date::floor<Duration>(std::chrono::system_clock::now()));
}

template<class Duration>
DateTime<typename DateTime<Duration>::CommonDuration> DateTime<Duration>::Now(const std::string &timezoneName) {
	if (timezoneName.empty()) {
		return Today();
	}
	return date::make_zoned(timezoneName, date::floor<Duration>(std::chrono::system_clock::now()));
}

template<class Duration>
DateTime<typename DateTime<Duration>::CommonDuration> DateTime<Duration>::UtcNow() {
	return {date::floor<Duration>(std::chrono::system_clock::now())};
}

template<class Duration>
template<class Rep>
DateTime<typename DateTime<Duration>::CommonDuration> DateTime<Duration>::FromTimestamp(Rep timestamp, const std::string &timezoneName) {
	auto nanos = static_cast<unsigned>(1e9 * std::fmod(timestamp, 1)); // might loose precision here
	auto tp = std::chrono::system_clock::from_time_t(timestamp) + std::chrono::nanoseconds(nanos);
	if (timezoneName.empty()) {
		return {date::make_zoned(date::current_zone(), tp)};
	} else {
		return {date::make_zoned(timezoneName, tp)};
	}
}

template<class Duration>
template<class Rep>
DateTime<typename DateTime<Duration>::CommonDuration> DateTime<Duration>::UtcFromTimestamp(Rep timestamp) {
	auto nanos = static_cast<unsigned>(1e9 * std::fmod(timestamp, 1)); // might loose precision here
	auto tp = std::chrono::system_clock::from_time_t(timestamp) + std::chrono::nanoseconds(nanos);
	return {tp};
}

template<class Duration>
DateTime<typename DateTime<Duration>::CommonDuration> DateTime<Duration>::Parse(const std::string &dateString, const std::string &format) {
	date::local_seconds tp;
	std::istringstream ss(dateString);
	ss >> date::parse(format, tp);
	auto zt = date::make_zoned(date::current_zone(), tp);
	return {zt};
}

template<class Duration>
bool DateTime<Duration>::TryParse(const std::string &dateString, const std::string &format, DateTime<CommonDuration> &dateTime) {
	date::local_seconds tp;
	std::istringstream ss(dateString);
	ss >> date::parse(format, tp);
	if (ss.fail()) return false;
	auto zt = date::make_zoned(date::current_zone(), tp);
	dateTime = {zt};
	return true;
}

template<class Duration>
std::optional<DateTime<typename DateTime<Duration>::CommonDuration>> DateTime<Duration>::TryParse(const std::string &dateString, const std::string &format) {
	DateTime<CommonDuration> dt;
	if (TryParse(dateString, format, dt))
		return dt;
	return {};
}

template<class Duration>
Date DateTime<Duration>::Date() const {
	return FieldsYmdTime().ymd;
}

template<class Duration>
date::year DateTime<Duration>::Year() const {
	return Date().Year();
}

template<class Duration>
date::month DateTime<Duration>::Month() const {
	return Date().Month();
}

template<class Duration>
date::day DateTime<Duration>::Day() const {
	return Date().Day();
}

template<class Duration>
const date::time_zone *DateTime<Duration>::Timezone() const {
	return _zt.get_time_zone();
}

template<class Duration>
const std::string &DateTime<Duration>::TzInfo() const {
	return _zt.get_time_zone()->name();
}

template<class Duration>
TimeDelta DateTime<Duration>::UtcOffset() const {
	auto offset = ZonedTime().get_info().offset;
	return {std::chrono::seconds{offset}};
}

template<class Duration>
std::string DateTime<Duration>::Timestamp() const {
	std::stringstream ss;
	ss << std::fixed << ZonedTime().get_sys_time().time_since_epoch().count() / 1000000000.0;
	return ss.str();
}

template<class Duration>
std::string DateTime<Duration>::Format(std::string_view format) const {
	return date::format(format.data(), _zt);
}

template<class Duration>
date::fields<typename DateTime<Duration>::CommonDuration> DateTime<Duration>::FieldsYmdTime() const {
	auto tp = ZonedTime().get_local_time();
	auto ld = date::floor<date::days>(tp);
	date::fields<CommonDuration> fds{date::year_month_day{ld}, date::time_of_day<CommonDuration>{tp - ld}};
	return fds;
}

template<class Duration>
DateTime<Duration> operator+(const DateTime<Duration> &x, const TimeDelta &y) {
	// TODO make this work for non default Duration
	auto add = x.ZonedTime().get_sys_time() + std::chrono::seconds(y.TotalSeconds()) + std::chrono::microseconds(y.Microseconds());
	return {date::make_zoned(x.ZonedTime().get_time_zone(), add)};
}

template<class Duration>
DateTime<Duration> operator+(const TimeDelta &y, const DateTime<Duration> &x) {
	return x + y;
}

template<class Duration>
DateTime<Duration> operator-(const DateTime<Duration> &x, const TimeDelta &y) {
	auto diff = x.ZonedTime().get_sys_time() - std::chrono::seconds(y.TotalSeconds()) - std::chrono::microseconds(y.Microseconds());
	return {date::make_zoned(x.ZonedTime().get_time_zone(), diff)};
}

template<class Duration>
TimeDelta operator-(const DateTime<Duration> &x, const DateTime<Duration> &y) {
	return {x.ZonedTime().get_sys_time() - y.ZonedTime().get_sys_time()};
}

template<class Duration1, class Duration2>
bool operator==(const DateTime<Duration1> &x, const DateTime<Duration2> &y) {
	return x.ZonedTime().get_sys_time() == y.ZonedTime().get_sys_time();
}

template<class Duration1, class Duration2>
bool operator!=(const DateTime<Duration1> &x, const DateTime<Duration2> &y) {
	return x.ZonedTime().get_sys_time() != y.ZonedTime().get_sys_time();
}

template<class Duration1, class Duration2>
bool operator<(const DateTime<Duration1> &x, const DateTime<Duration2> &y) {
	return x.ZonedTime().get_sys_time() < y.ZonedTime().get_sys_time();
}

template<class Duration1, class Duration2>
bool operator<=(const DateTime<Duration1> &x, const DateTime<Duration2> &y) {
	return x.ZonedTime().get_sys_time() <= y.ZonedTime().get_sys_time();
}

template<class Duration1, class Duration2>
bool operator>(const DateTime<Duration1> &x, const DateTime<Duration2> &y) {
	return x.ZonedTime().get_sys_time() > y.ZonedTime().get_sys_time();
}

template<class Duration1, class Duration2>
bool operator>=(const DateTime<Duration1> &x, const DateTime<Duration2> &y) {
	return x.ZonedTime().get_sys_time() >= y.ZonedTime().get_sys_time();
}

template<class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os, const DateTime<Duration> &date) {
	return os << date.ZonedTime();
}
}
