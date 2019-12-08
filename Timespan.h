#pragma once

#include "Timestamp.h"

namespace Poco {
/// A class that represents time spans up to microsecond resolution.
class Timespan {
public:
	using TimeDiff = Timestamp::TimeDiff;

	/// Creates a zero Timespan.
	Timespan();

	/// Creates a Timespan.
	Timespan(TimeDiff microseconds);

	/// Creates a Timespan. Useful for creating
	/// a Timespan from a struct timeval.
	Timespan(long seconds, long microseconds);

	/// Creates a Timespan.
	Timespan(int days, int hours, int minutes, int seconds, int microseconds);

	/// Creates a Timespan from another one.
	Timespan(const Timespan &timespan);

	/// Destroys the Timespan.
	~Timespan() = default;

	/// Returns the number of days.
	int GetDays() const;

	/// Returns the number of hours (0 to 23).
	int GetHours() const;

	/// Returns the total number of hours.
	int GetTotalHours() const;

	/// Returns the number of minutes (0 to 59).
	int GetMinutes() const;

	/// Returns the total number of minutes.
	int GetTotalMinutes() const;

	/// Returns the number of seconds (0 to 59).
	int GetSeconds() const;

	/// Returns the total number of seconds.
	int GetTotalSeconds() const;

	/// Returns the number of milliseconds (0 to 999).
	int GetMilliseconds() const;

	/// Returns the total number of milliseconds.
	TimeDiff GetTotalMilliseconds() const;

	/// Returns the fractions of a millisecond
	/// in microseconds (0 to 999).
	int GetMicroseconds() const;

	/// Returns the fractions of a second
	/// in microseconds (0 to 999999).
	int GetNanoseconds() const;

	/// Returns the total number of microseconds.
	TimeDiff GetTotalMicroseconds() const;

	/// Assigns a Timespan.
	Timespan &operator=(const Timespan &timespan) = default;

	bool operator==(const Timespan &ts) const;
	bool operator!=(const Timespan &ts) const;
	bool operator>(const Timespan &ts) const;
	bool operator>=(const Timespan &ts) const;
	bool operator<(const Timespan &ts) const;
	bool operator<=(const Timespan &ts) const;

	Timespan operator+(const Timespan &d) const;
	Timespan operator-(const Timespan &d) const;
	Timespan &operator+=(const Timespan &d);
	Timespan &operator-=(const Timespan &d);

	Timespan &operator=(TimeDiff microseconds);
	
	bool operator==(TimeDiff microseconds) const;
	bool operator!=(TimeDiff microseconds) const;
	bool operator>(TimeDiff microseconds) const;
	bool operator>=(TimeDiff microseconds) const;
	bool operator<(TimeDiff microseconds) const;
	bool operator<=(TimeDiff microseconds) const;

	Timespan operator+(TimeDiff microseconds) const;
	Timespan operator-(TimeDiff microseconds) const;
	Timespan &operator+=(TimeDiff microseconds);
	Timespan &operator-=(TimeDiff microseconds);

	static const TimeDiff MILLISECONDS; /// The number of microseconds in a millisecond.
	static const TimeDiff SECONDS; /// The number of microseconds in a second.
	static const TimeDiff MINUTES; /// The number of microseconds in a minute.
	static const TimeDiff HOURS; /// The number of microseconds in a hour.
	static const TimeDiff DAYS; /// The number of microseconds in a day.

private:
	TimeDiff _span;
};
}
