#include "DateTime.hpp"

#include "Timespan.hpp"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace Poco {
DateTime::DateTime() {
	Timestamp now;
	_utcTime = now.GetUtcTime();
	ComputeGregorian(GetJulianDay());
	ComputeDaytime();
}

DateTime::DateTime(const Timestamp &timestamp) :
	_utcTime(timestamp.GetUtcTime()) {
	ComputeGregorian(GetJulianDay());
	ComputeDaytime();
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond) :
	_year(year),
	_month(month),
	_day(day),
	_hour(hour),
	_minute(minute),
	_second(second),
	_millisecond(millisecond),
	_microsecond(microsecond) {
	assert(year >= 0 && year <= 9999);
	assert(month >= 1 && month <= 12);
	assert(day >= 1 && day <= GetDaysOfMonth(year, month));
	assert(hour >= 0 && hour <= 23);
	assert(minute >= 0 && minute <= 59);
	assert(second >= 0 && second <= 59);
	assert(millisecond >= 0 && millisecond <= 999);
	assert(microsecond >= 0 && microsecond <= 999);

	_utcTime = ToUtcTime(ToJulianDay(year, month, day)) + 10 * (hour * Timespan::HOURS + minute * Timespan::MINUTES + second * Timespan::SECONDS + millisecond * Timespan::
		MILLISECONDS + microsecond);
}

DateTime::DateTime(double julianDay) :
	_utcTime(ToUtcTime(julianDay)) {
	ComputeGregorian(julianDay);
}

DateTime::DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff) :
	_utcTime(utcTime + diff * 10) {
	ComputeGregorian(GetJulianDay());
	ComputeDaytime();
}

DateTime::DateTime(const DateTime &dateTime) :
	_utcTime(dateTime._utcTime),
	_year(dateTime._year),
	_month(dateTime._month),
	_day(dateTime._day),
	_hour(dateTime._hour),
	_minute(dateTime._minute),
	_second(dateTime._second),
	_millisecond(dateTime._millisecond),
	_microsecond(dateTime._microsecond) {
}

int DateTime::GetYear() const {
	return _year;
}

int DateTime::GetMonth() const {
	return _month;
}

int DateTime::GetWeek(int firstDayOfWeek) const {
	assert(firstDayOfWeek >= 0 && firstDayOfWeek <= 6);

	/// find the first firstDayOfWeek.
	int baseDay = 1;
	while (DateTime(_year, 1, baseDay).GetDayOfWeek() != firstDayOfWeek) ++baseDay;

	int doy = GetDayOfYear();
	int offs = baseDay <= 4 ? 0 : 1;
	if (doy < baseDay)
		return offs;
	return (doy - baseDay) / 7 + 1 + offs;
}

int DateTime::GetDay() const {
	return _day;
}

int DateTime::GetDayOfWeek() const {
	return int((std::floor(GetJulianDay() + 1.5))) % 7;
}

int DateTime::GetDayOfYear() const {
	int doy = 0;
	for (int month = 1; month < _month; ++month)
		doy += GetDaysOfMonth(_year, month);
	doy += _day;
	return doy;
}

int DateTime::GetHour() const {
	return _hour;
}

int DateTime::GetHourAmPm() const {
	if (_hour < 1)
		return 12;
	if (_hour > 12)
		return _hour - 12;
	return _hour;
}

bool DateTime::IsAm() const {
	return _hour < 12;
}

bool DateTime::IsPm() const {
	return _hour >= 12;
}

int DateTime::GetMinute() const {
	return _minute;
}

int DateTime::GetSecond() const {
	return _second;
}

int DateTime::GetMillisecond() const {
	return _millisecond;
}

int DateTime::GetMicrosecond() const {
	return _microsecond;
}

double DateTime::GetJulianDay() const {
	return ToJulianDay(_utcTime);
}

Timestamp DateTime::GetTimestamp() const {
	return Timestamp::FromUtcTime(_utcTime);
}

Timestamp::UtcTimeVal DateTime::GetUtcTime() const {
	return _utcTime;
}

void DateTime::MakeUtc(int tzd) {
	operator -=(Timespan(static_cast<Timestamp::TimeDiff>(tzd) * Timespan::SECONDS));
}

void DateTime::MakeLocal(int tzd) {
	operator +=(Timespan(static_cast<Timestamp::TimeDiff>(tzd) * Timespan::SECONDS));
}

DateTime &DateTime::operator =(const DateTime &dateTime) {
	if (&dateTime != this) {
		_utcTime = dateTime._utcTime;
		_year = dateTime._year;
		_month = dateTime._month;
		_day = dateTime._day;
		_hour = dateTime._hour;
		_minute = dateTime._minute;
		_second = dateTime._second;
		_millisecond = dateTime._millisecond;
		_microsecond = dateTime._microsecond;
	}
	return *this;
}

DateTime &DateTime::operator=(const Timestamp &timestamp) {
	_utcTime = timestamp.GetUtcTime();
	ComputeGregorian(GetJulianDay());
	ComputeDaytime();
	return *this;
}

DateTime &DateTime::operator=(double julianDay) {
	_utcTime = ToUtcTime(julianDay);
	ComputeGregorian(julianDay);
	return *this;
}

bool DateTime::operator==(const DateTime &dateTime) const {
	return _utcTime == dateTime._utcTime;
}

bool DateTime::operator!=(const DateTime &dateTime) const {
	return _utcTime != dateTime._utcTime;
}

bool DateTime::operator<(const DateTime &dateTime) const {
	return _utcTime < dateTime._utcTime;
}

bool DateTime::operator<=(const DateTime &dateTime) const {
	return _utcTime <= dateTime._utcTime;
}

bool DateTime::operator>(const DateTime &dateTime) const {
	return _utcTime > dateTime._utcTime;
}

bool DateTime::operator>=(const DateTime &dateTime) const {
	return _utcTime >= dateTime._utcTime;
}

DateTime DateTime::operator+(const Timespan &span) const {
	return DateTime(_utcTime, span.GetTotalMicroseconds());
}

DateTime DateTime::operator-(const Timespan &span) const {
	return DateTime(_utcTime, -span.GetTotalMicroseconds());
}

Timespan DateTime::operator-(const DateTime &dateTime) const {
	return Timespan((_utcTime - dateTime._utcTime) / 10);
}

DateTime &DateTime::operator+=(const Timespan &span) {
	_utcTime += span.GetTotalMicroseconds() * 10;
	ComputeGregorian(GetJulianDay());
	ComputeDaytime();
	return *this;
}

DateTime &DateTime::operator-=(const Timespan &span) {
	_utcTime -= span.GetTotalMicroseconds() * 10;
	ComputeGregorian(GetJulianDay());
	ComputeDaytime();
	return *this;
}

bool DateTime::IsLeapYear(int year) {
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}

int DateTime::GetDaysOfMonth(int year, int month) {
	assert(month >= 1 && month <= 12);

	static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month == 2 && IsLeapYear(year))
		return 29;
	return daysOfMonthTable[month];
}

bool DateTime::IsValid(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond) {
	return
		(year >= 0 && year <= 9999) &&
		(month >= 1 && month <= 12) &&
		(day >= 1 && day <= GetDaysOfMonth(year, month)) &&
		(hour >= 0 && hour <= 23) &&
		(minute >= 0 && minute <= 59) &&
		(second >= 0 && second <= 59) &&
		(millisecond >= 0 && millisecond <= 999) &&
		(microsecond >= 0 && microsecond <= 999);
}

double DateTime::ToJulianDay(Timestamp::UtcTimeVal utcTime) {
	double utcDays = double(utcTime) / 864000000000.0;
	return utcDays + 2299160.5; // first day of Gregorian reform (Oct 15 1582)
}

double DateTime::ToJulianDay(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond) {
	// lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
	static int lookup[] = {-91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337};

	// day to double
	double dday = double(day) + ((double((hour * 60 + minute) * 60 + second) * 1000 + millisecond) * 1000 + microsecond) / 86400000000.0;
	if (month < 3) {
		month += 12;
		--year;
	}
	double dyear = double(year);
	return dday + lookup[month] + 365 * year + std::floor(dyear / 4) - std::floor(dyear / 100) + std::floor(dyear / 400) + 1721118.5;
}

Timestamp::UtcTimeVal DateTime::ToUtcTime(double julianDay) {
	return Timestamp::UtcTimeVal((julianDay - 2299160.5) * 864000000000.0);
}

void DateTime::ComputeGregorian(double julianDay) {
	double z = std::floor(julianDay - 1721118.5);
	double r = julianDay - 1721118.5 - z;
	double g = z - 0.25;
	double a = std::floor(g / 36524.25);
	double b = a - std::floor(a / 4);
	_year = short(std::floor((b + g) / 365.25));
	double c = b + z - std::floor(365.25 * _year);
	_month = short(std::floor((5 * c + 456) / 153));
	double dday = c - std::floor((153.0 * _month - 457) / 5) + r;
	_day = short(dday);
	if (_month > 12) {
		++_year;
		_month -= 12;
	}
	r *= 24;
	_hour = short(std::floor(r));
	r -= std::floor(r);
	r *= 60;
	_minute = short(std::floor(r));
	r -= std::floor(r);
	r *= 60;
	_second = short(std::floor(r));
	r -= std::floor(r);
	r *= 1000;
	_millisecond = short(std::floor(r));
	r -= std::floor(r);
	r *= 1000;
	_microsecond = short(r + 0.5);

	Normalize();

	assert(_month >= 1 && _month <= 12);
	assert(_day >= 1 && _day <= GetDaysOfMonth(_year, _month));
	assert(_hour >= 0 && _hour <= 23);
	assert(_minute >= 0 && _minute <= 59);
	assert(_second >= 0 && _second <= 59);
	assert(_millisecond >= 0 && _millisecond <= 999);
	assert(_microsecond >= 0 && _microsecond <= 999);
}

void DateTime::ComputeDaytime() {
	Timespan span(_utcTime / 10);
	int hour = span.GetHours();
	// Due to double rounding issues, the previous call to ComputeGregorian()
	// may have crossed into the next or previous day. We need to correct that.
	if (hour == 23 && _hour == 0) {
		_day--;
		if (_day == 0) {
			_month--;
			if (_month == 0) {
				_month = 12;
				_year--;
			}
			_day = GetDaysOfMonth(_year, _month);
		}
	} else if (hour == 0 && _hour == 23) {
		_day++;
		if (_day > GetDaysOfMonth(_year, _month)) {
			_month++;
			if (_month > 12) {
				_month = 1;
				_year++;
			}
			_day = 1;
		}
	}
	_hour = hour;
	_minute = span.GetMinutes();
	_second = span.GetSeconds();
	_millisecond = span.GetMilliseconds();
	_microsecond = span.GetMicroseconds();
}

void DateTime::CheckLimit(short &lower, short &higher, short limit) {
	if (lower >= limit) {
		higher += short(lower / limit);
		lower = short(lower % limit);
	}
}

void DateTime::Normalize() {
	CheckLimit(_microsecond, _millisecond, 1000);
	CheckLimit(_millisecond, _second, 1000);
	CheckLimit(_second, _minute, 60);
	CheckLimit(_minute, _hour, 60);
	CheckLimit(_hour, _day, 24);

	if (_day > GetDaysOfMonth(_year, _month)) {
		_day -= GetDaysOfMonth(_year, _month);
		if (++_month > 12) {
			++_year;
			_month -= 12;
		}
	}
}
}
