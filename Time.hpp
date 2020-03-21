#pragma once

#include <iomanip>
#include <date/date.h>

namespace datetime {
class Time {
public:
	template<class Duration>
	Time(const Duration &dur) :
		_timeOfDay(date::make_time(std::chrono::duration_cast<std::chrono::system_clock::duration>(dur))) {
	}

	template<class Duration, class ... Durations>
	Time(const Duration &d, const Durations &... durations) :
		_timeOfDay(date::make_time(std::chrono::duration_cast<std::chrono::system_clock::duration>(AddDurations(d, durations...)))) {
	}

	const date::time_of_day<std::chrono::system_clock::duration> &TimeOfDay() const { return _timeOfDay; }

	std::chrono::hours::rep Hour() const { return TimeOfDay().hours().count(); }
	std::chrono::minutes::rep Minute() const { return TimeOfDay().minutes().count(); }
	std::chrono::seconds::rep Seconds() const { return TimeOfDay().seconds().count(); }

	std::string Format(std::string_view format = "%H:%M:%S") const {
		std::tm tm;
		tm.tm_sec = static_cast<int>(Seconds());
		tm.tm_min = static_cast<int>(Minute());
		tm.tm_hour = static_cast<int>(Hour());
		// No milli nor microseconds :(
		std::stringstream buffer;
		buffer << std::put_time(&tm, format.data());
		return buffer.str();
	}

private:
	template<class Duration>
	static Duration AddDurations(const Duration &d) {
		return d;
	}

	template<class Duration, class ... Durations>
	auto AddDurations(const Duration &d, const Durations &... durations) ->
		typename std::common_type<Duration, Durations...>::type {
		return d + AddDurations(durations...);
	}

	date::time_of_day<std::chrono::system_clock::duration> _timeOfDay;
};

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os, const Time &time) {
	return os << time.TimeOfDay();
}
}
