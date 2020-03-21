#include <iostream>

#include "LocalDateTime.hpp"
#include "DateTime.hpp"
#include "DateTimeFormat.hpp"
#include "DateTimeFormatter.hpp"
#include "DateTimeParser.hpp"
#include "Timezone.hpp"
#include "Timespan.hpp"
#include <cassert>

using namespace Poco;

void testISO8601_Format() {
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::Format(dt, DateTimeFormat::ISO8601_FORMAT);
	assert(str == "2005-01-08T12:30:00Z");

	str = DateTimeFormatter::Format(dt, DateTimeFormat::ISO8601_FORMAT, 3600);
	assert(str == "2005-01-08T12:30:00+01:00");

	str = DateTimeFormatter::Format(dt, DateTimeFormat::ISO8601_FORMAT, -3600);
	assert(str == "2005-01-08T12:30:00-01:00");
}

void testISO8601_Parse() {
	int tzd;
	DateTime dt = DateTimeParser::Parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00Z", tzd);
	assert(dt.GetYear() == 2005);
	assert(dt.GetMonth() == 1);
	assert(dt.GetDay() == 8);
	assert(dt.GetHour() == 12);
	assert(dt.GetMinute() == 30);
	assert(dt.GetSecond() == 0);
	assert(tzd == 0);

	dt = DateTimeParser::Parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00+01:00", tzd);
	assert(dt.GetYear() == 2005);
	assert(dt.GetMonth() == 1);
	assert(dt.GetDay() == 8);
	assert(dt.GetHour() == 12);
	assert(dt.GetMinute() == 30);
	assert(dt.GetSecond() == 0);
	assert(tzd == 3600);

	dt = DateTimeParser::Parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00-01:00", tzd);
	assert(dt.GetYear() == 2005);
	assert(dt.GetMonth() == 1);
	assert(dt.GetDay() == 8);
	assert(dt.GetHour() == 12);
	assert(dt.GetMinute() == 30);
	assert(dt.GetSecond() == 0);
	assert(tzd == -3600);

	dt = DateTimeParser::Parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00", tzd);
	assert(dt.GetYear() == 2005);
	assert(dt.GetMonth() == 1);
	assert(dt.GetDay() == 8);
	assert(dt.GetHour() == 12);
	assert(dt.GetMinute() == 30);
	assert(dt.GetSecond() == 0);
	assert(tzd == 0);

	dt = DateTimeParser::Parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08", tzd);
	assert(dt.GetYear() == 2005);
	assert(dt.GetMonth() == 1);
	assert(dt.GetDay() == 8);
	assert(dt.GetHour() == 0);
	assert(dt.GetMinute() == 0);
	assert(dt.GetSecond() == 0);
	assert(tzd == 0);
}

void testTimestamp() {
	Timestamp ts(0); // Unix epoch 1970-01-01 00:00:00 Thursday
	DateTime dt(ts);
	assert(dt.GetYear() == 1970);
	assert(dt.GetMonth() == 1);
	assert(dt.GetDay() == 1);
	assert(dt.GetHour() == 0);
	assert(dt.GetMinute() == 0);
	assert(dt.GetSecond() == 0);
	assert(dt.GetMillisecond() == 0);
	assert(dt.GetDayOfWeek() == 4);
	assert(dt.GetJulianDay() == 2440587.5);
	assert(dt.GetTimestamp() == 0);

	ts = Timestamp::FromEpochTime(1000000000);
	dt = ts; // 2001-09-09 01:46:40 Sunday
	assert(dt.GetYear() == 2001);
	assert(dt.GetMonth() == 9);
	assert(dt.GetDay() == 9);
	assert(dt.GetHour() == 1);
	assert(dt.GetMinute() == 46);
	assert(dt.GetSecond() == 40);
	assert(dt.GetMillisecond() == 0);
	assert(dt.GetDayOfWeek() == 0);
	assert(dt.GetTimestamp().GetEpochTime() == 1000000000);
}

void testConversions() {
	Timespan ts;
	assert(ts.GetTotalMicroseconds() == 0);
	ts = Timespan::DAYS;
	assert(ts.GetTotalMicroseconds() == Timespan::DAYS);
	assert(ts.GetTotalMilliseconds() == 86400000);
	assert(ts.GetTotalSeconds() == 86400);
	assert(ts.GetTotalMinutes() == 60 * 24);
	assert(ts.GetTotalHours() == 24);
	assert(ts.GetDays() == 1);

	assert(ts.GetMicroseconds() == 0);
	assert(ts.GetMilliseconds() == 0);
	assert(ts.GetSeconds() == 0);
	assert(ts.GetMinutes() == 0);
	assert(ts.GetHours() == 0);

	ts = Timespan(2, 12, 30, 10, 123456);
	assert(ts.GetMicroseconds() == 456);
	assert(ts.GetMilliseconds() == 123);
	assert(ts.GetSeconds() == 10);
	assert(ts.GetMinutes() == 30);
	assert(ts.GetHours() == 12);
	assert(ts.GetDays() == 2);

	ts = Timespan(0, 36, 30, 10, 123456);
	assert(ts.GetMicroseconds() == 456);
	assert(ts.GetMilliseconds() == 123);
	assert(ts.GetNanoseconds() == 123456);
	assert(ts.GetSeconds() == 10);
	assert(ts.GetMinutes() == 30);
	assert(ts.GetHours() == 12);
	assert(ts.GetDays() == 1);

	ts = Timespan(0, 0, 2190, 10, 123456);
	assert(ts.GetMicroseconds() == 456);
	assert(ts.GetMilliseconds() == 123);
	assert(ts.GetNanoseconds() == 123456);
	assert(ts.GetSeconds() == 10);
	assert(ts.GetMinutes() == 30);
	assert(ts.GetHours() == 12);
	assert(ts.GetDays() == 1);
}

void testTimezone() {
	std::string name = Timezone::Name();
	std::string stdName = Timezone::StandardName();
	std::string dstName = Timezone::DstName();
	std::cout << "Timezone Names: " << name << ", " << stdName << ", " << dstName << std::endl;
	int utcOffset = Timezone::UtcOffset();
	std::cout << "UTC Offset: " << utcOffset << std::endl;
	int dst = Timezone::Dst();
	std::cout << "DST Offset: " << dst << std::endl;
}

void testGregorian() {
	LocalDateTime dt(1970, 1, 1);
	assert(dt.GetYear() == 1970);
	assert(dt.GetMonth() == 1);
	assert(dt.GetDay() == 1);
	assert(dt.GetHour() == 0);
	assert(dt.GetMinute() == 0);
	assert(dt.GetSecond() == 0);
	assert(dt.GetMillisecond() == 0);
	assert(dt.GetDayOfWeek() == 4);
	// REMOVED: this fails when the current DST offset differs from
	//          the one on 1970-1-1
	//assert (dt.GetTzd() == Timezone::tzd());
	assert(dt.GetJulianDay() == 2440587.5);

	dt = LocalDateTime(2001, 9, 9, 1, 46, 40);
	assert(dt.GetYear() == 2001);
	assert(dt.GetMonth() == 9);
	assert(dt.GetDay() == 9);
	assert(dt.GetHour() == 1);
	assert(dt.GetMinute() == 46);
	assert(dt.GetSecond() == 40);
	assert(dt.GetMillisecond() == 0);
	assert(dt.GetDayOfWeek() == 0);
	//assert (dt.GetTzd() == Timezone::tzd());
}

int main() {
	testISO8601_Format();
	testISO8601_Parse();
	testTimestamp();
	testConversions();
	testTimezone();
	testGregorian();

	LocalDateTime now;

	std::string str = DateTimeFormatter::Format(now, DateTimeFormat::ISO8601_FORMAT);
	std::cout << str << '\n';
	DateTime dt;
	int tzd;
	DateTimeParser::Parse(DateTimeFormat::ISO8601_FORMAT, str, dt, tzd);
	dt.MakeUtc(tzd);
	LocalDateTime ldt(tzd, dt);
	std::cout << DateTimeFormatter::Format(dt, DateTimeFormat::ISO8601_FORMAT) << '\n';
	return 0;
}
