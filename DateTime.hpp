#pragma once

#include "Timestamp.hpp"
#include "Timespan.hpp"

namespace Poco {
/// This class represents an instant in time, expressed
/// in years, months, days, hours, minutes, seconds
/// and milliseconds based on the Gregorian calendar.
/// The class is mainly useful for conversions between
/// UTC, Julian day and Gregorian calendar dates.
///
/// The date and time stored in a DateTime is always in UTC
/// (Coordinated Universal Time) and thus independent of the 
/// timezone in effect on the system.
///
/// Conversion calculations are based on algorithms
/// collected and described by Peter Baum at
/// http://vsg.cape.com/~pbaum/date/date0.htm
///
/// Internally, this class stores a date/time in two
/// forms (UTC and broken down) for performance reasons. Only use 
/// this class for conversions between date/time representations.
/// Use the Timestamp class for everything else.
///
/// Notes:
///   * Zero is a valid year (in accordance with ISO 8601 and astronomical year numbering)
///   * Year zero (0) is a leap year
///   * Negative years (years preceding 1 BC) are not supported
///
/// For more information, please see:
///   * http://en.wikipedia.org/wiki/Gregorian_Calendar
///   * http://en.wikipedia.org/wiki/Julian_day
///   * http://en.wikipedia.org/wiki/UTC
///   * http://en.wikipedia.org/wiki/ISO_8601
class DateTime {
public:
	/// Symbolic names for month numbers (1 to 12).
	enum Months {
		JANUARY = 1,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};

	/// Symbolic names for week day numbers (0 to 6).
	enum DaysOfWeek {
		SUNDAY = 0,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};

	/// Creates a DateTime for the current date and time.
	DateTime();

	/// Creates a DateTime for the date and time given in
	/// a Timestamp.
	DateTime(const Timestamp &timestamp);

	/// Creates a DateTime for the given Gregorian date and time.
	///   * year is from 0 to 9999.
	///   * month is from 1 to 12.
	///   * day is from 1 to 31.
	///   * hour is from 0 to 23.
	///   * minute is from 0 to 59.
	///   * second is from 0 to 60 (allowing leap seconds).
	///   * millisecond is from 0 to 999.
	///   * microsecond is from 0 to 999.
	DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

	/// Creates a DateTime for the given Julian day.
	DateTime(double julianDay);

	/// Creates a DateTime from an UtcTimeVal and a TimeDiff.
	///
	/// Mainly used internally by DateTime and friends.
	DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff);

	/// Copy constructor. Creates the DateTime from another one.
	DateTime(const DateTime &dateTime);

	/// Destroys the DateTime.
	~DateTime() = default;

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
	int GetWeek(int firstDayOfWeek = MONDAY) const;

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

	/// Returns the julian day for the date and time.
	double GetJulianDay() const;

	/// Returns the date and time expressed as a Timestamp.
	Timestamp GetTimestamp() const;

	/// Returns the date and time expressed in UTC-based
	/// time. UTC base time is midnight, October 15, 1582.
	/// Resolution is 100 nanoseconds.
	Timestamp::UtcTimeVal GetUtcTime() const;

	/// Converts a local time into UTC, by applying the given time zone differential.
	void MakeUtc(int tzd);

	/// Converts a UTC time into a local time, by applying the given time zone differential.
	void MakeLocal(int tzd);

	/// Assigns another DateTime.
	DateTime &operator=(const DateTime &dateTime);
	/// Assigns a Timestamp.
	DateTime &operator=(const Timestamp &timestamp);
	/// Assigns a Julian day.
	DateTime &operator=(double julianDay);

	bool operator==(const DateTime &dateTime) const;
	bool operator!=(const DateTime &dateTime) const;
	bool operator<(const DateTime &dateTime) const;
	bool operator<=(const DateTime &dateTime) const;
	bool operator>(const DateTime &dateTime) const;
	bool operator>=(const DateTime &dateTime) const;

	DateTime operator+(const Timespan &span) const;
	DateTime operator-(const Timespan &span) const;
	Timespan operator-(const DateTime &dateTime) const;
	DateTime &operator+=(const Timespan &span);
	DateTime &operator-=(const Timespan &span);

	/// Returns true if the given year is a leap year;
	/// false otherwise.
	static bool IsLeapYear(int year);

	/// Returns the number of days in the given month
	/// and year. Month is from 1 to 12.
	static int GetDaysOfMonth(int year, int month);

	/// Checks if the given date and time is valid
	/// (all arguments are within a proper range).
	///
	/// Returns true if all arguments are valid, false otherwise.
	static bool IsValid(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

protected:
	/// Computes the Julian day for an UTC time.
	static double ToJulianDay(Timestamp::UtcTimeVal utcTime);

	/// Computes the Julian day for a gregorian calendar date and time.
	/// See <http://vsg.cape.com/~pbaum/date/jdimp.htm>, section 2.3.1 for the algorithm.
	static double ToJulianDay(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

	/// Computes the UTC time for a Julian day.
	static Timestamp::UtcTimeVal ToUtcTime(double julianDay);

	/// Computes the Gregorian date for the given Julian day.
	/// See <http://vsg.cape.com/~pbaum/date/injdimp.htm>, section 3.3.1 for the algorithm.
	void ComputeGregorian(double julianDay);

	/// Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.
	void ComputeDaytime();

private:
	static void CheckLimit(short &lower, short &higher, short limit);
	/// Utility functions used to correct the overflow in computeGregorian
	void Normalize();

	Timestamp::UtcTimeVal _utcTime;
	short _year;
	short _month;
	short _day;
	short _hour;
	short _minute;
	short _second;
	short _millisecond;
	short _microsecond;
};
}
