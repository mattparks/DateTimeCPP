#pragma once

#include <ctime>
#include <cstdint>

namespace Poco {
class Timespan;
/// A Timestamp stores a monotonic* time value
/// with (theoretical) microseconds resolution.
/// Timestamps can be compared with each other
/// and simple arithmetics are supported.
///
/// [*] Note that Timestamp values are only monotonic as
/// long as the systems's clock is monotonic as well
/// (and not, e.g. set back).
///
/// Timestamps are UTC (Coordinated Universal Time)
/// based and thus independent of the timezone
/// in effect on the system.
class Timestamp {
public:
	using TimeVal = int64_t; /// monotonic UTC time value in microsecond resolution
	using UtcTimeVal = int64_t; /// monotonic UTC time value in 100 nanosecond resolution
	using TimeDiff = int64_t; /// difference between two timestamps in microseconds

	static const TimeVal Min;
	static const TimeVal Max;

	/// Creates a timestamp with the current time.
	Timestamp();

	/// Creates a timestamp from the given time value.
	Timestamp(TimeVal tv);

	/// Copy constructor.
	Timestamp(const Timestamp &other);

	/// Destroys the Timestamp.
	~Timestamp() = default;

	/// Updates the Timestamp with the current time.
	void Update();

	/// Returns the timestamp expressed in time_t.
	/// time_t base time is midnight, January 1, 1970.
	/// Resolution is one second.
	std::time_t GetEpochTime() const;

	/// Returns the timestamp expressed in UTC-based
	/// time. UTC base time is midnight, October 15, 1582.
	/// Resolution is 100 nanoseconds.
	UtcTimeVal GetUtcTime() const;

	/// Returns the timestamp expressed in microseconds
	/// since the Unix epoch, midnight, January 1, 1970.
	TimeVal GetEpochMicroseconds() const;

	/// Returns the time elapsed since the time denoted by
	/// the timestamp. Equivalent to Timestamp() - *this.
	TimeDiff GetElapsed() const;

	/// Returns true iff the given interval has passed
	/// since the time denoted by the timestamp.
	bool IsElapsed(TimeDiff interval) const;

	/// Assigns a Timestamp.
	Timestamp &operator=(const Timestamp &other) = default;
	/// Assigns a TimeVal.
	Timestamp &operator=(TimeVal tv);

	bool operator==(const Timestamp &ts) const;
	bool operator!=(const Timestamp &ts) const;
	bool operator>(const Timestamp &ts) const;
	bool operator>=(const Timestamp &ts) const;
	bool operator<(const Timestamp &ts) const;
	bool operator<=(const Timestamp &ts) const;

	Timestamp operator+(TimeDiff d) const;
	Timestamp operator+(const Timespan &span) const;
	Timestamp operator-(TimeDiff d) const;
	Timestamp operator-(const Timespan &span) const;
	TimeDiff operator-(const Timestamp &ts) const;
	Timestamp &operator+=(TimeDiff d);
	Timestamp &operator+=(const Timespan &span);
	Timestamp &operator-=(TimeDiff d);
	Timestamp &operator-=(const Timespan &span);

	/// Creates a timestamp from a std::time_t.
	static Timestamp FromEpochTime(std::time_t t);

	/// Creates a timestamp from a UTC time value.
	static Timestamp FromUtcTime(UtcTimeVal val);

	/// Returns the resolution in units per second.
	/// Since the timestamp has microsecond resolution,
	/// the returned value is always 1000000.
	static TimeVal GetResolution();

private:
	TimeVal _ts;
};
}
