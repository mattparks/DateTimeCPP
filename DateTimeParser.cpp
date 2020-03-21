#include "DateTimeParser.hpp"

#include <stdexcept>

#include "DateTimeFormat.hpp"
#include "DateTime.hpp"

namespace Poco {
#define SKIP_JUNK() \
	while (it != end && !std::isdigit(*it)) ++it

#define SKIP_DIGITS() \
	while (it != end && std::isdigit(*it)) ++it

#define PARSE_NUMBER(var) \
	while (it != end && std::isdigit(*it)) var = var*10 + ((*it++) - '0')

#define PARSE_NUMBER_N(var, n) \
	{ int i = 0; while (i++ < n && it != end && std::isdigit(*it)) var = var*10 + ((*it++) - '0'); }

#define PARSE_FRACTIONAL_N(var, n) \
	{ int i = 0; while (i < n && it != end && std::isdigit(*it)) { var = var*10 + ((*it++) - '0'); i++; } while (i++ < n) var *= 10; }

void DateTimeParser::Parse(std::string_view fmt, std::string_view str, DateTime &dateTime, int &timeZoneDifferential) {
	if (fmt.empty() || str.empty())
		throw std::runtime_error("Empty string");

	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	int millis = 0;
	int micros = 0;
	int tzd = 0;

	std::string_view::const_iterator it = str.begin();
	std::string_view::const_iterator end = str.end();
	std::string_view::const_iterator itf = fmt.begin();
	std::string_view::const_iterator endf = fmt.end();

	while (itf != endf && it != end) {
		if (*itf == '%') {
			if (++itf != endf) {
				switch (*itf) {
				case 'w':
				case 'W':
					while (it != end && std::isspace(*it)) ++it;
					while (it != end && std::isalpha(*it)) ++it;
					break;
				case 'b':
				case 'B':
					month = ParseMonth(it, end);
					break;
				case 'd':
				case 'e':
				case 'f':
					SKIP_JUNK();
					PARSE_NUMBER_N(day, 2);
					break;
				case 'm':
				case 'n':
				case 'o':
					SKIP_JUNK();
					PARSE_NUMBER_N(month, 2);
					break;
				case 'y':
					SKIP_JUNK();
					PARSE_NUMBER_N(year, 2);
					if (year >= 69)
						year += 1900;
					else
						year += 2000;
					break;
				case 'Y':
					SKIP_JUNK();
					PARSE_NUMBER_N(year, 4);
					break;
				case 'r':
					SKIP_JUNK();
					PARSE_NUMBER(year);
					if (year < 1000) {
						if (year >= 69)
							year += 1900;
						else
							year += 2000;
					}
					break;
				case 'H':
				case 'h':
					SKIP_JUNK();
					PARSE_NUMBER_N(hour, 2);
					break;
				case 'a':
				case 'A':
					hour = ParseAmPm(it, end, hour);
					break;
				case 'M':
					SKIP_JUNK();
					PARSE_NUMBER_N(minute, 2);
					break;
				case 'S':
					SKIP_JUNK();
					PARSE_NUMBER_N(second, 2);
					break;
				case 's':
					SKIP_JUNK();
					PARSE_NUMBER_N(second, 2);
					if (it != end && (*it == '.' || *it == ',')) {
						++it;
						PARSE_FRACTIONAL_N(millis, 3);
						PARSE_FRACTIONAL_N(micros, 3);
						SKIP_DIGITS();
					}
					break;
				case 'i':
					SKIP_JUNK();
					PARSE_NUMBER_N(millis, 3);
					break;
				case 'c':
					SKIP_JUNK();
					PARSE_NUMBER_N(millis, 1);
					millis *= 100;
					break;
				case 'F':
					SKIP_JUNK();
					PARSE_FRACTIONAL_N(millis, 3);
					PARSE_FRACTIONAL_N(micros, 3);
					SKIP_DIGITS();
					break;
				case 'z':
				case 'Z':
					tzd = ParseTzd(it, end);
					break;
				}
				++itf;
			}
		} else ++itf;
	}
	if (month == 0) month = 1;
	if (day == 0) day = 1;
	if (DateTime::IsValid(year, month, day, hour, minute, second, millis, micros))
		dateTime = DateTime(year, month, day, hour, minute, second, millis, micros);
	else
		throw std::runtime_error("date/time component out of range");
	timeZoneDifferential = tzd;
}

DateTime DateTimeParser::Parse(std::string_view fmt, std::string_view str, int &timeZoneDifferential) {
	DateTime result;
	Parse(fmt, str, result, timeZoneDifferential);
	return result;
}

bool DateTimeParser::TryParse(std::string_view fmt, std::string_view str, DateTime &dateTime, int &timeZoneDifferential) {
	try {
		Parse(fmt, str, dateTime, timeZoneDifferential);
	} catch (std::exception &) {
		return false;
	}
	return true;
}

void DateTimeParser::Parse(std::string_view str, DateTime &dateTime, int &timeZoneDifferential) {
	if (!TryParse(str, dateTime, timeZoneDifferential))
		throw std::runtime_error("Unsupported or invalid date/time format");
}

DateTime DateTimeParser::Parse(std::string_view str, int &timeZoneDifferential) {
	DateTime result;
	if (TryParse(str, result, timeZoneDifferential))
		return result;
	throw std::runtime_error("Unsupported or invalid date/time format");
}

bool DateTimeParser::TryParse(std::string_view str, DateTime &dateTime, int &timeZoneDifferential) {
	if (str.length() < 4) return false;

	if (str[3] == ',')
		return TryParse("%w, %e %b %r %H:%M:%S %Z", str, dateTime, timeZoneDifferential);
	if (str[3] == ' ')
		return TryParse(DateTimeFormat::ASCTIME_FORMAT, str, dateTime, timeZoneDifferential);
	if (str.find(',') < 10)
		return TryParse("%W, %e %b %r %H:%M:%S %Z", str, dateTime, timeZoneDifferential);
	if (std::isdigit(str[0])) {
		if (str.find(' ') != std::string::npos || str.length() == 10)
			return TryParse(DateTimeFormat::SORTABLE_FORMAT, str, dateTime, timeZoneDifferential);
		if (str.find('.') != std::string::npos || str.find(',') != std::string::npos)
			return TryParse(DateTimeFormat::ISO8601_FRAC_FORMAT, str, dateTime, timeZoneDifferential);
		return TryParse(DateTimeFormat::ISO8601_FORMAT, str, dateTime, timeZoneDifferential);
	}
	return false;
}

int DateTimeParser::ParseTzd(std::string_view::const_iterator &it, const std::string_view::const_iterator &end) {
	struct Zone {
		const char *designator;
		int timeZoneDifferential;
	};

	static Zone zones[] = {
		{"Z", 0},
		{"UT", 0},
		{"GMT", 0},
		{"BST", 1 * 3600},
		{"IST", 1 * 3600},
		{"WET", 0},
		{"WEST", 1 * 3600},
		{"CET", 1 * 3600},
		{"CEST", 2 * 3600},
		{"EET", 2 * 3600},
		{"EEST", 3 * 3600},
		{"MSK", 3 * 3600},
		{"MSD", 4 * 3600},
		{"NST", -3 * 3600 - 1800},
		{"NDT", -2 * 3600 - 1800},
		{"AST", -4 * 3600},
		{"ADT", -3 * 3600},
		{"EST", -5 * 3600},
		{"EDT", -4 * 3600},
		{"CST", -6 * 3600},
		{"CDT", -5 * 3600},
		{"MST", -7 * 3600},
		{"MDT", -6 * 3600},
		{"PST", -8 * 3600},
		{"PDT", -7 * 3600},
		{"AKST", -9 * 3600},
		{"AKDT", -8 * 3600},
		{"HST", -10 * 3600},
		{"AEST", 10 * 3600},
		{"AEDT", 11 * 3600},
		{"ACST", 9 * 3600 + 1800},
		{"ACDT", 10 * 3600 + 1800},
		{"AWST", 8 * 3600},
		{"AWDT", 9 * 3600}
	};

	int tzd = 0;
	while (it != end && std::isspace(*it)) ++it;
	if (it != end) {
		if (std::isalpha(*it)) {
			std::string designator;
			designator += *it++;
			if (it != end && std::isalpha(*it)) designator += *it++;
			if (it != end && std::isalpha(*it)) designator += *it++;
			if (it != end && std::isalpha(*it)) designator += *it++;
			for (auto &zone : zones) {
				if (designator == zone.designator){
					tzd = zone.timeZoneDifferential;
					break;
				}
			}
		}
		if (it != end && (*it == '+' || *it == '-')) {
			int sign = *it == '+' ? 1 : -1;
			++it;
			int hours = 0;
			PARSE_NUMBER_N(hours, 2);
			if (it != end && *it == ':') ++it;
			int minutes = 0;
			PARSE_NUMBER_N(minutes, 2);
			tzd += sign * (hours * 3600 + minutes * 60);
		}
	}
	return tzd;
}

int DateTimeParser::ParseMonth(std::string_view::const_iterator &it, const std::string_view::const_iterator &end) {
	std::string month;
	while (it != end && (std::isspace(*it) || std::ispunct(*it))) ++it;
	bool isFirst = true;
	while (it != end && std::isalpha(*it)) {
		char ch = (*it++);
		if (isFirst) {
			month += std::toupper(ch);
			isFirst = false;
		} else month += std::tolower(ch);
	}
	if (month.length() < 3) throw std::runtime_error("Month name must be at least three characters long"); //, month);
	for (int i = 0; i < 12; ++i) {
		if (DateTimeFormat::MONTH_NAMES[i].find(month) == 0)
			return i + 1;
	}
	throw std::runtime_error("Not a valid month name"); //, month);
}

int DateTimeParser::ParseDayOfWeek(std::string_view::const_iterator &it, const std::string_view::const_iterator &end) {
	std::string dow;
	while (it != end && (std::isspace(*it) || std::ispunct(*it))) ++it;
	bool isFirst = true;
	while (it != end && std::isalpha(*it)) {
		char ch = (*it++);
		if (isFirst) {
			dow += std::toupper(ch);
			isFirst = false;
		} else dow += std::tolower(ch);
	}
	if (dow.length() < 3) throw std::runtime_error("Weekday name must be at least three characters long"); //, dow);
	for (int i = 0; i < 7; ++i) {
		if (DateTimeFormat::WEEKDAY_NAMES[i].find(dow) == 0)
			return i;
	}
	throw std::runtime_error("Not a valid weekday name"); //, dow);
}

int DateTimeParser::ParseAmPm(std::string_view::const_iterator &it, const std::string_view::const_iterator &end, int hour) {
	std::string ampm;
	while (it != end && (std::isspace(*it) || std::ispunct(*it))) ++it;
	while (it != end && std::isalpha(*it)) {
		char ch = (*it++);
		ampm += std::toupper(ch);
	}
	if (ampm == "AM") {
		if (hour == 12)
			return 0;
		return hour;
	}
	if (ampm == "PM") {
		if (hour < 12)
			return hour + 12;
		return hour;
	}
	throw std::runtime_error("Not a valid AM/PM designator"); //, ampm);
}
}
