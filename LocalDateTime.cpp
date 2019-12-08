#include "LocalDateTime.h"

#include <ctime>
#include <stdexcept>

#include "Timezone.h"
#include "Timespan.h"

namespace Poco {
LocalDateTime::LocalDateTime() {
	DetermineTzd(true);
}

LocalDateTime::LocalDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond) :
	_dateTime(year, month, day, hour, minute, second, millisecond, microsecond) {
	DetermineTzd();
}

LocalDateTime::LocalDateTime(int tzd, int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond) :
	_dateTime(year, month, day, hour, minute, second, millisecond, microsecond),
	_tzd(tzd) {}

LocalDateTime::LocalDateTime(double julianDay) :
	_dateTime(julianDay) {
	DetermineTzd(true);
}

LocalDateTime::LocalDateTime(int tzd, double julianDay) :
	_dateTime(julianDay),
	_tzd(tzd) {
	AdjustForTzd();
}

LocalDateTime::LocalDateTime(const DateTime &dateTime) :
	_dateTime(dateTime) {
	DetermineTzd(true);
}

LocalDateTime::LocalDateTime(int tzd, const DateTime &dateTime) :
	_dateTime(dateTime),
	_tzd(tzd) {
	AdjustForTzd();
}

LocalDateTime::LocalDateTime(int tzd, const DateTime &dateTime, bool adjust) :
	_dateTime(dateTime),
	_tzd(tzd) {
	if (adjust)
		AdjustForTzd();
}

LocalDateTime::LocalDateTime(const LocalDateTime &dateTime) :
	_dateTime(dateTime._dateTime),
	_tzd(dateTime._tzd) {
}

LocalDateTime::LocalDateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff, int tzd) :
	_dateTime(utcTime, diff),
	_tzd(tzd) {
	AdjustForTzd();
}

int LocalDateTime::GetYear() const {
	return _dateTime.GetYear();
}

int LocalDateTime::GetMonth() const {
	return _dateTime.GetMonth();
}

int LocalDateTime::GetWeek(int firstDayOfWeek) const {
	return _dateTime.GetWeek(firstDayOfWeek);
}

int LocalDateTime::GetDay() const {
	return _dateTime.GetDay();
}

int LocalDateTime::GetDayOfWeek() const {
	return _dateTime.GetDayOfWeek();
}

int LocalDateTime::GetDayOfYear() const {
	return _dateTime.GetDayOfYear();
}

int LocalDateTime::GetHour() const {
	return _dateTime.GetHour();
}

int LocalDateTime::GetHourAmPm() const {
	return _dateTime.GetHourAmPm();
}

bool LocalDateTime::IsAm() const {
	return _dateTime.IsAm();
}

bool LocalDateTime::IsPm() const {
	return _dateTime.IsPm();
}

int LocalDateTime::GetMinute() const {
	return _dateTime.GetMinute();
}

int LocalDateTime::GetSecond() const {
	return _dateTime.GetSecond();
}

int LocalDateTime::GetMillisecond() const {
	return _dateTime.GetMillisecond();
}

int LocalDateTime::GetMicrosecond() const {
	return _dateTime.GetMicrosecond();
}

double LocalDateTime::GetJulianDay() const {
	return _dateTime.GetJulianDay();
}

int LocalDateTime::GetTzd() const {
	return _tzd;
}

DateTime LocalDateTime::GetUtc() const {
	return DateTime(_dateTime.GetUtcTime(), -static_cast<Timestamp::TimeDiff>(_tzd) * Timespan::SECONDS);
}

Timestamp LocalDateTime::GetTimestamp() const {
	return Timestamp::FromUtcTime(_dateTime.GetUtcTime());
}

Timestamp::UtcTimeVal LocalDateTime::GetUtcTime() const {
	return _dateTime.GetUtcTime() - static_cast<Timestamp::TimeDiff>(_tzd) * 10000000;
}

LocalDateTime &LocalDateTime::operator=(const LocalDateTime &dateTime) {
	if (&dateTime != this) {
		_dateTime = dateTime._dateTime;
		_tzd = dateTime._tzd;
	}
	return *this;
}

LocalDateTime &LocalDateTime::operator=(const Timestamp &timestamp) {
	if (timestamp != this->GetTimestamp()) {
		_dateTime = timestamp;
		DetermineTzd(true);
	}
	return *this;
}

LocalDateTime &LocalDateTime::operator=(double julianDay) {
	_dateTime = julianDay;
	DetermineTzd(true);
	return *this;
}

bool LocalDateTime::operator==(const LocalDateTime &dateTime) const {
	return GetUtcTime() == dateTime.GetUtcTime();
}

bool LocalDateTime::operator!=(const LocalDateTime &dateTime) const {
	return GetUtcTime() != dateTime.GetUtcTime();
}

bool LocalDateTime::operator<(const LocalDateTime &dateTime) const {
	return GetUtcTime() < dateTime.GetUtcTime();
}

bool LocalDateTime::operator<=(const LocalDateTime &dateTime) const {
	return GetUtcTime() <= dateTime.GetUtcTime();
}

bool LocalDateTime::operator>(const LocalDateTime &dateTime) const {
	return GetUtcTime() > dateTime.GetUtcTime();
}

bool LocalDateTime::operator>=(const LocalDateTime &dateTime) const {
	return GetUtcTime() >= dateTime.GetUtcTime();
}

LocalDateTime LocalDateTime::operator+(const Timespan &span) const {
	// First calculate the adjusted UTC time, then calculate the
	// locally adjusted time by constructing a new LocalDateTime.
	DateTime tmp(GetUtcTime(), span.GetTotalMicroseconds());
	return LocalDateTime(tmp);
}

LocalDateTime LocalDateTime::operator-(const Timespan &span) const {
	// First calculate the adjusted UTC time, then calculate the
	// locally adjusted time by constructing a new LocalDateTime.
	DateTime tmp(GetUtcTime(), -span.GetTotalMicroseconds());
	return LocalDateTime(tmp);
}

Timespan LocalDateTime::operator-(const LocalDateTime &dateTime) const {
	return Timespan((GetUtcTime() - dateTime.GetUtcTime()) / 10);
}

LocalDateTime &LocalDateTime::operator+=(const Timespan &span) {
	// Use the same trick as in operator+. Create a UTC time, adjust
	// it for the span, and convert back to LocalDateTime. This will
	// recalculate the tzd correctly in the case where the addition
	// crosses a DST boundary.
	*this = DateTime(GetUtcTime(), span.GetTotalMicroseconds());
	return *this;
}

LocalDateTime &LocalDateTime::operator-=(const Timespan &span) {
	// Use the same trick as in operator-. Create a UTC time, adjust
	// it for the span, and convert back to LocalDateTime. This will
	// recalculate the tzd correctly in the case where the subtraction
	// crosses a DST boundary.
	*this = DateTime(GetUtcTime(), -span.GetTotalMicroseconds());
	return *this;
}

void LocalDateTime::DetermineTzd(bool adjust) {
	if (adjust) {
		std::time_t epochTime = _dateTime.GetTimestamp().GetEpochTime();
#if defined(_WIN32)
		std::tm *broken = std::localtime(&epochTime);
		if (!broken) throw std::runtime_error("cannot get local time");
		_tzd = (Timezone::UtcOffset() + ((broken->tm_isdst == 1) ? 3600 : 0));
#else
		std::tm broken;
		if (!localtime_r(&epochTime, &broken))
			throw Poco::SystemException("cannot get local time");
		_tzd = (Timezone::utcOffset() + ((broken.tm_isdst == 1) ? 3600 : 0));
#endif
		AdjustForTzd();
	} else {
		int dst;
		DstOffset(dst);
		_tzd = (Timezone::UtcOffset() + dst);
	}
}

void LocalDateTime::AdjustForTzd() {
	_dateTime += Timespan(static_cast<Timestamp::TimeDiff>(_tzd) *Timespan::SECONDS);
}

std::time_t LocalDateTime::DstOffset(int &dstOffset) const {
	std::time_t local;
	std::tm broken;

	broken.tm_year = _dateTime.GetYear() - 1900;
	broken.tm_mon = _dateTime.GetMonth() - 1;
	broken.tm_mday = _dateTime.GetDay();
	broken.tm_hour = _dateTime.GetHour();
	broken.tm_min = _dateTime.GetMinute();
	broken.tm_sec = _dateTime.GetSecond();
	broken.tm_isdst = -1;
	local = std::mktime(&broken);

	dstOffset = (broken.tm_isdst == 1) ? 3600 : 0;
	return local;
}
}
