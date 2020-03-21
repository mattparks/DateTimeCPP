#include "DateTimeFormatter.hpp"

#include <iomanip>

#include "DateTimeFormat.hpp"

namespace Poco {
std::string DateTimeFormatter::Format(const Timestamp &timestamp, std::string_view fmt, int timeZoneDifferential) {
	DateTime dateTime(timestamp);
	return Format(dateTime, fmt, timeZoneDifferential);
}

std::string DateTimeFormatter::Format(const DateTime &dateTime, std::string_view fmt, int timeZoneDifferential) {
	std::ostringstream result;
	Append(result, dateTime, fmt, timeZoneDifferential);
	return result.str();
}

std::string DateTimeFormatter::Format(const LocalDateTime &dateTime, std::string_view fmt) {
	return Format(dateTime._dateTime, fmt, dateTime._tzd);
}

std::string DateTimeFormatter::Format(const Timespan &timespan, std::string_view fmt) {
	std::ostringstream result;
	Append(result, timespan, fmt);
	return result.str();
}

void DateTimeFormatter::Append(std::ostringstream &str, const Timestamp &timestamp, std::string_view fmt, int timeZoneDifferential) {
	DateTime dateTime(timestamp);
	Append(str, dateTime, fmt, timeZoneDifferential);
}

std::string DateTimeFormatter::TzdISO(int timeZoneDifferential) {
	std::ostringstream result;
	TzdISO(result, timeZoneDifferential);
	return result.str();
}

std::string DateTimeFormatter::TzdRFC(int timeZoneDifferential) {
	std::ostringstream result;
	TzdRFC(result, timeZoneDifferential);
	return result.str();
}

void DateTimeFormatter::Append(std::ostringstream &str, const LocalDateTime &dateTime, std::string_view fmt) {
	Append(str, dateTime._dateTime, fmt, dateTime.GetTzd());
}

void DateTimeFormatter::Append(std::ostringstream &str, const DateTime &dateTime, std::string_view fmt, int timeZoneDifferential) {
	std::string_view::const_iterator it = fmt.begin();
	std::string_view::const_iterator end = fmt.end();
	while (it != end) {
		if (*it == '%') {
			if (++it != end) {
				switch (*it) {
				case 'w':
					str << DateTimeFormat::WEEKDAY_NAMES[dateTime.GetDayOfWeek()].substr(0, 3);
					break;
				case 'W':
					str << DateTimeFormat::WEEKDAY_NAMES[dateTime.GetDayOfWeek()];
					break;
				case 'b':
					str << DateTimeFormat::MONTH_NAMES[dateTime.GetMonth() - 1].substr(0, 3);
					break;
				case 'B':
					str << DateTimeFormat::MONTH_NAMES[dateTime.GetMonth() - 1];
					break;
				case 'd':
					str << std::setfill('0') << std::setw(2) << dateTime.GetDay();
					break;
				case 'e':
					str << dateTime.GetDay();
					break;
				case 'f':
					str << std::setw(2) << dateTime.GetDay();
					break;
				case 'm':
					str << std::setfill('0') << std::setw(2) << dateTime.GetMonth();
					break;
				case 'n':
					str << dateTime.GetMonth();
					break;
				case 'o':
					str << std::setw(2) << dateTime.GetMonth();
					break;
				case 'y':
					str << std::setfill('0') << std::setw(2) << (dateTime.GetYear() % 100);
					break;
				case 'Y':
					str << std::setfill('0') << std::setw(4) << dateTime.GetYear();
					break;
				case 'H':
					str << std::setfill('0') << std::setw(2) << dateTime.GetHour();
					break;
				case 'h':
					str << std::setfill('0') << std::setw(2) << dateTime.GetHourAmPm();
					break;
				case 'a':
					str << (dateTime.IsAm() ? "am" : "pm");
					break;
				case 'A':
					str << (dateTime.IsAm() ? "AM" : "PM");
					break;
				case 'M':
					str << std::setfill('0') << std::setw(2) << dateTime.GetMinute();
					break;
				case 'S':
					str << std::setfill('0') << std::setw(2) << dateTime.GetSecond();
					break;
				case 's':
					str << std::setfill('0') << std::setw(2) << dateTime.GetSecond();
					str << '.';
					str << std::setfill('0') << std::setw(6) << (dateTime.GetMillisecond() * 1000 + dateTime.GetMicrosecond());
					break;
				case 'i':
					str << std::setfill('0') << std::setw(3) << dateTime.GetMillisecond();
					break;
				case 'c':
					str << (dateTime.GetMillisecond() / 100);
					break;
				case 'F':
					str << std::setfill('0') << std::setw(6) << (dateTime.GetMillisecond() * 1000 + dateTime.GetMicrosecond());
					break;
				case 'z':
					TzdISO(str, timeZoneDifferential);
					break;
				case 'Z':
					TzdRFC(str, timeZoneDifferential);
					break;
				default:
					str << *it;
				}
				++it;
			}
		} else {
			str << *it++;
		}
	}
}

void DateTimeFormatter::Append(std::ostringstream &str, const Timespan &timespan, std::string_view fmt) {
	std::string_view::const_iterator it = fmt.begin();
	std::string_view::const_iterator end = fmt.end();
	while (it != end) {
		if (*it == '%') {
			if (++it != end) {
				switch (*it) {
				case 'd':
					str << timespan.GetDays();
					break;
				case 'H':
					str << std::setfill('0') << std::setw(2) << timespan.GetHours();
					break;
				case 'h':
					str << timespan.GetTotalHours();
					break;
				case 'M':
					str << std::setfill('0') << std::setw(2) << timespan.GetMinutes();
					break;
				case 'm':
					str << timespan.GetTotalMinutes();
					break;
				case 'S':
					str << std::setfill('0') << std::setw(2) << timespan.GetSeconds();
					break;
				case 's':
					str << timespan.GetTotalSeconds();
					break;
				case 'i':
					str << std::setfill('0') << std::setw(3) << timespan.GetMilliseconds();
					break;
				case 'c':
					str << (timespan.GetMilliseconds() / 100);
					break;
				case 'F':
					str << std::setfill('0') << std::setw(6) << (timespan.GetMilliseconds() * 1000 + timespan.GetMicroseconds());
					break;
				default:
					str << *it;
				}
				++it;
			}
		} else {
			str << *it++;
		}
	}
}

void DateTimeFormatter::TzdISO(std::ostringstream &str, int timeZoneDifferential) {
	if (timeZoneDifferential != UTC) {
		if (timeZoneDifferential >= 0) {
			str << '+';
			str << std::setfill('0') << std::setw(2) << (timeZoneDifferential / 3600);
			str << ':';
			str << std::setfill('0') << std::setw(2) << ((timeZoneDifferential % 3600) / 60);
		} else {
			str << '-';
			str << std::setfill('0') << std::setw(2) << (-timeZoneDifferential / 3600);
			str << ':';
			str << std::setfill('0') << std::setw(2) << ((-timeZoneDifferential % 3600) / 60);
		}
	} else {
		str << 'Z';
	}
}

void DateTimeFormatter::TzdRFC(std::ostringstream &str, int timeZoneDifferential) {
	if (timeZoneDifferential != UTC) {
		if (timeZoneDifferential >= 0) {
			str << '+';
			str << std::setfill('0') << std::setw(2) << (timeZoneDifferential / 3600, 2);
			str << std::setfill('0') << std::setw(2) << (timeZoneDifferential % 3600);
		} else {
			str << '-';
			str << std::setfill('0') << std::setw(2) << (-timeZoneDifferential / 3600);
			str << std::setfill('0') << std::setw(2) << ((-timeZoneDifferential % 3600) / 60);
		}
	} else {
		str << "GMT";
	}
}
}
