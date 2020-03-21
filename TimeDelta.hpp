#pragma once

#include <cmath>
#include <date/date.h>

namespace datetime {
class TimeDelta {
public:
	// TODO there is a problem when using date::months
	template <class Duration>
	TimeDelta(const Duration &d) :
		_days(std::chrono::duration_cast<date::days>(d)),
		_seconds(std::chrono::duration_cast<std::chrono::seconds>(d - _days)),
		_microseconds(std::chrono::duration_cast<std::chrono::microseconds>(d - _days - _seconds)) {
	}

	template<class Duration, class ... Durations>
	TimeDelta(const Duration &d, const Durations &... durations) :
		TimeDelta(d) { // use delegate ctor here
		AddDelta(durations...);
	}

	date::days::rep Days() const { return _days.count(); }
	std::chrono::seconds::rep Seconds() const { return _seconds.count(); }
	std::chrono::microseconds::rep Microseconds() const { return _microseconds.count(); }

	std::chrono::seconds::rep TotalSeconds() const {
		return std::chrono::duration_cast<std::chrono::seconds>(_days + _seconds).count();
	}

private:
	template<class Duration>
	void AddDelta(const Duration &d) {
		auto totalDuration = _days + _seconds + _microseconds + d;
		_days = std::chrono::duration_cast<date::days>(totalDuration);
		_seconds = std::chrono::duration_cast<std::chrono::seconds>(totalDuration - _days);
		_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(totalDuration - _days - _seconds);
	}

	template<class Duration, class ... Durations>
	void AddDelta(const Duration &d, const Durations &... durations) {
		AddDelta(d);
		AddDelta(durations...);
	}

	date::days _days;
	std::chrono::seconds _seconds;
	std::chrono::microseconds _microseconds;
};

inline TimeDelta operator+(const TimeDelta &x, const TimeDelta &y) {
	return {
		date::days(x.Days() + y.Days()),
		std::chrono::seconds(x.Seconds() + y.Seconds()),
		std::chrono::microseconds(x.Microseconds() + y.Microseconds())
	};
}

inline TimeDelta operator-(const TimeDelta &x, const TimeDelta &y) {
	return {
		date::days(x.Days() - y.Days()),
		std::chrono::seconds(x.Seconds() - y.Seconds()),
		std::chrono::microseconds(x.Microseconds() - y.Microseconds())
	};
}

template<class Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type * = nullptr>
TimeDelta operator*(Scalar s, const TimeDelta &x) {
	return {
		std::chrono::seconds(std::lrint(s * x.TotalSeconds())), // round-half-to-even
		std::chrono::microseconds(std::lrint(s * x.Microseconds()))
	};
}

template<class Scalar, typename std::enable_if<std::is_integral<Scalar>::value>::type * = nullptr>
TimeDelta operator*(Scalar s, const TimeDelta &x) {
	return {
		date::days(s * static_cast<unsigned>(x.Days())),
		std::chrono::seconds(s * x.Seconds()),
		std::chrono::microseconds(s * x.Microseconds())
	};
}

template<class Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type * = nullptr>
TimeDelta operator*(const TimeDelta &x, Scalar s) {
	return s * x;
}

template<class Scalar, typename std::enable_if<std::is_integral<Scalar>::value>::type * = nullptr>
TimeDelta operator*(const TimeDelta &x, Scalar s) {
	return s * x;
}

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os, const TimeDelta &td) {
	// return os << '(' << td.days() << " days, " << td.seconds() << " s, " << td.microseconds() << " µs)";
	return os << td.Days() << " days, " << date::make_time(std::chrono::seconds(td.Seconds()) + std::chrono::microseconds(td.Microseconds()));
}
}
