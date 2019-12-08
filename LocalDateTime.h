#pragma once

#include "DateTime.h"

namespace Poco {
/// This class represents an instant in local time
/// (as opposed to UTC), expressed in years, months, days, 
/// hours, minutes, seconds and milliseconds based on the 
/// Gregorian calendar.
///
/// In addition to the date and time, the class also 
/// maintains a time zone differential, which denotes
/// the difference in seconds from UTC to local time,
/// i.e. UTC = local time - time zone differential.
///
/// Although LocalDateTime supports relational and arithmetic
/// operators, all date/time comparisons and date/time arithmetics
/// should be done in UTC, using the DateTime or Timestamp
/// class for better performance. The relational operators
/// normalize the dates/times involved to UTC before carrying out
/// the comparison.
///
/// The time zone differential is based on the input date and
/// time and current time zone. A number of constructors accept
/// an explicit time zone differential parameter. These should
/// not be used since daylight savings time processing is impossible
/// since the time zone is unknown. Each of the constructors
/// accepting a tzd parameter have been marked as deprecated and
/// may be removed in a future revision.
class LocalDateTime {
	friend class DateTimeFormatter;
	friend class DateTimeParser;
public:
	/// Creates a LocalDateTime with the current date/time 
	/// for the current time zone.
	LocalDateTime();

	/// Creates a LocalDateTime for the given Gregorian local date and time.
	///   * year is from 0 to 9999.
	///   * month is from 1 to 12.
	///   * day is from 1 to 31.
	///   * hour is from 0 to 23.
	///   * minute is from 0 to 59.
	///   * second is from 0 to 60 (allowing leap seconds).
	///   * millisecond is from 0 to 999.
	///   * microsecond is from 0 to 999.
	LocalDateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

	//@ deprecated
	/// Creates a LocalDateTime for the given Gregorian date and time in the
	/// time zone denoted by the time zone differential in tzd.
	///   * tzd is in seconds.
	///   * year is from 0 to 9999.
	///   * month is from 1 to 12.
	///   * day is from 1 to 31.
	///   * hour is from 0 to 23.
	///   * minute is from 0 to 59.
	///   * second is from 0 to 60 (allowing leap seconds).
	///   * millisecond is from 0 to 999.
	///   * microsecond is from 0 to 999.
	LocalDateTime(int tzd, int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond);

	/// Creates a LocalDateTime from the UTC time given in dateTime,
	/// using the time zone differential of the current time zone.
	LocalDateTime(const DateTime &dateTime);

	//@ deprecated
	/// Creates a LocalDateTime from the UTC time given in dateTime,
	/// using the given time zone differential. Adjusts dateTime
	/// for the given time zone differential.
	LocalDateTime(int tzd, const DateTime &dateTime);

	//@ deprecated
	/// Creates a LocalDateTime from the UTC time given in dateTime,
	/// using the given time zone differential. If adjust is true, 
	/// adjusts dateTime for the given time zone differential.
	LocalDateTime(int tzd, const DateTime &dateTime, bool adjust);

	/// Creates a LocalDateTime for the given Julian day in the local time zone.
	LocalDateTime(double julianDay);

	//@ deprecated
	/// Creates a LocalDateTime for the given Julian day in the time zone
	/// denoted by the time zone differential in tzd.
	LocalDateTime(int tzd, double julianDay);

	/// Copy constructor. Creates the LocalDateTime from another one.
	LocalDateTime(const LocalDateTime &dateTime);

	/// Destroys the LocalDateTime.
	~LocalDateTime() = default;

	/// Returns the year.
	int GetYear() const;

	/// Returns the month (1 to 12).
	int GetMonth() const;

	/// Returns the week number within the year.
	/// FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1).
	/// The returned week number will be from 0 to 53. Week number 1 is the week 
	/// containing January 4. This is in accordance to ISO 8601.
	/// 
	/// The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started
	/// on a Saturday, week 1 will be the week starting on Monday, January 3.
	/// January 1 and 2 will fall within week 0 (or the last week of the previous year).
	///
	/// For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1.
	/// There will be no week 0 in 2007.
	int GetWeek(int firstDayOfWeek = DateTime::MONDAY) const;

	/// Returns the day witin the month (1 to 31).
	int GetDay() const;

	/// Returns the weekday (0 to 6, where
	/// 0 = Sunday, 1 = Monday, ..., 6 = Saturday).
	int GetDayOfWeek() const;

	/// Returns the number of the day in the year.
	/// January 1 is 1, February 1 is 32, etc.
	int GetDayOfYear() const;

	/// Returns the hour (0 to 23).
	int GetHour() const;

	/// Returns the hour (0 to 12).
	int GetHourAmPm() const;

	/// Returns true if hour < 12;
	bool IsAm() const;

	/// Returns true if hour >= 12.
	bool IsPm() const;

	/// Returns the minute (0 to 59).
	int GetMinute() const;

	/// Returns the second (0 to 59).
	int GetSecond() const;

	/// Returns the millisecond (0 to 999)
	int GetMillisecond() const;

	/// Returns the microsecond (0 to 999)
	int GetMicrosecond() const;

	/// Returns the julian day for the date.
	double GetJulianDay() const;

	/// Returns the time zone differential.
	int GetTzd() const;

	/// Returns the UTC equivalent for the local date and time.
	DateTime GetUtc() const;

	/// Returns the date and time expressed as a Timestamp.
	Timestamp GetTimestamp() const;

	/// Returns the UTC equivalent for the local date and time.
	Timestamp::UtcTimeVal GetUtcTime() const;

	/// Assigns another LocalDateTime.
	LocalDateTime &operator=(const LocalDateTime &dateTime);

	/// Assigns a timestamp
	LocalDateTime &operator=(const Timestamp &timestamp);

	/// Assigns a Julian day in the local time zone.
	LocalDateTime &operator=(double julianDay);

	bool operator==(const LocalDateTime &dateTime) const;
	bool operator!=(const LocalDateTime &dateTime) const;
	bool operator<(const LocalDateTime &dateTime) const;
	bool operator<=(const LocalDateTime &dateTime) const;
	bool operator>(const LocalDateTime &dateTime) const;
	bool operator>=(const LocalDateTime &dateTime) const;

	LocalDateTime operator+(const Timespan &span) const;
	LocalDateTime operator-(const Timespan &span) const;
	Timespan operator-(const LocalDateTime &dateTime) const;
	LocalDateTime &operator+=(const Timespan &span);
	LocalDateTime &operator-=(const Timespan &span);

protected:
	LocalDateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff, int tzd);

	/// Recalculate the tzd based on the _dateTime member based
	/// on the current timezone using the Standard C runtime functions.
	/// If adjust is true, then adjustForTzd() is called after the
	/// differential is calculated.
	void DetermineTzd(bool adjust = false);

	/// Adjust the _dateTime member based on the _tzd member.
	void AdjustForTzd();

	/// Determine the DST offset for the current date/time.
	std::time_t DstOffset(int &dstOffset) const;

private:
	DateTime _dateTime;
	int _tzd;
};
}
