#pragma once

#include <string>
#include <sstream>

#include "DateTime.hpp"
#include "LocalDateTime.hpp"

namespace Poco {
class Timestamp;
class Timespan;

/// This class converts dates and times into strings, supporting a  
/// variety of standard and custom formats.
///
/// There are two kind of static member functions:
///    * format* functions return a std::string containing
///      the formatted value.
///    * append* functions append the formatted value to
///      an existing string.
class DateTimeFormatter {
public:
	enum {
		UTC = 0xFFFF /// Special value for timeZoneDifferential denoting UTC. 
	};

	/// Formats the given timestamp according to the given format.
	/// The format string is used as a template to format the date and
	/// is copied character by character except for the following special characters,
	/// which are replaced by the corresponding value.
	///
	///   * %w - abbreviated weekday (Mon, Tue, ...)
	///   * %W - full weekday (Monday, Tuesday, ...)
	///   * %b - abbreviated month (Jan, Feb, ...)
	///   * %B - full month (January, February, ...)
	///   * %d - zero-padded day of month (01 .. 31)
	///   * %e - day of month (1 .. 31)
	///   * %f - space-padded day of month ( 1 .. 31)
	///   * %m - zero-padded month (01 .. 12)
	///   * %n - month (1 .. 12)
	///   * %o - space-padded month ( 1 .. 12)
	///   * %y - year without century (70)
	///   * %Y - year with century (1970)
	///   * %H - hour (00 .. 23)
	///   * %h - hour (00 .. 12)
	///   * %a - am/pm
	///   * %A - AM/PM
	///   * %M - minute (00 .. 59)
	///   * %S - second (00 .. 59)
	///   * %s - seconds and microseconds (equivalent to %S.%F)
	///   * %i - millisecond (000 .. 999)
	///   * %c - centisecond (0 .. 9)
	///   * %F - fractional seconds/microseconds (000000 - 999999)
	///   * %z - time zone differential in ISO 8601 format (Z or +NN.NN)
	///   * %Z - time zone differential in RFC format (GMT or +NNNN)
	///   * %% - percent sign
	///
	/// Class DateTimeFormat defines format strings for various standard date/time formats.
	static std::string Format(const Timestamp &timestamp, std::string_view fmt, int timeZoneDifferential = UTC);

	/// Formats the given date and time according to the given format.
	/// See format(const Timestamp&, const std::string&, int) for more information.
	static std::string Format(const DateTime &dateTime, std::string_view fmt, int timeZoneDifferential = UTC);

	/// Formats the given local date and time according to the given format.
	/// See format(const Timestamp&, const std::string&, int) for more information.
	static std::string Format(const LocalDateTime &dateTime, std::string_view fmt);

	/// Formats the given timespan according to the given format.
	/// The format string is used as a template to format the date and
	/// is copied character by character except for the following special characters,
	/// which are replaced by the corresponding value.
	///
	///   * %d - days
	///   * %H - hours	 (00 .. 23)
	///   * %h - total hours (0 .. n)
	///   * %M - minutes (00 .. 59)
	///   * %m - total minutes (0 .. n)
	///   * %S - seconds (00 .. 59)
	///   * %s - total seconds (0 .. n)
	///   * %i - milliseconds (000 .. 999)
	///   * %c - centisecond (0 .. 9)
	///   * %F - fractional seconds/microseconds (000000 - 999999)
	///   * %% - percent sign
	static std::string Format(const Timespan &timespan, std::string_view fmt = "%dd %H:%M:%S.%i");

	/// Formats the given timestamp according to the given format and appends it to str.
	///
	/// See format() for documentation of the formatting string.
	static void Append(std::ostringstream &str, const Timestamp &timestamp, std::string_view fmt, int timeZoneDifferential = UTC);

	/// Formats the given date and time according to the given format and appends it to str.
	///
	/// See format() for documentation of the formatting string.
	static void Append(std::ostringstream &str, const DateTime &dateTime, std::string_view fmt, int timeZoneDifferential = UTC);

	/// Formats the given local date and time according to the given format and appends it to str.
	///
	/// See format() for documentation of the formatting string.
	static void Append(std::ostringstream &str, const LocalDateTime &dateTime, std::string_view fmt);

	/// Formats the given timespan according to the given format and appends it to str.
	///
	/// See format() for documentation of the formatting string.
	static void Append(std::ostringstream &str, const Timespan &timespan, std::string_view fmt = "%dd %H:%M:%S.%i");

	/// Formats the given timezone differential in ISO format.
	/// If timeZoneDifferential is UTC, "Z" is returned,
	/// otherwise, +HH.MM (or -HH.MM) is returned.
	static std::string TzdISO(int timeZoneDifferential);

	/// Formats the given timezone differential in RFC format.
	/// If timeZoneDifferential is UTC, "GMT" is returned,
	/// otherwise ++HHMM (or -HHMM) is returned.
	static std::string TzdRFC(int timeZoneDifferential);

	/// Formats the given timezone differential in ISO format
	/// and appends it to the given string.
	/// If timeZoneDifferential is UTC, "Z" is returned,
	/// otherwise, +HH.MM (or -HH.MM) is returned.
	static void TzdISO(std::ostringstream &str, int timeZoneDifferential);

	/// Formats the given timezone differential in RFC format
	/// and appends it to the given string.
	/// If timeZoneDifferential is UTC, "GMT" is returned,
	/// otherwise ++HHMM (or -HHMM) is returned.
	static void TzdRFC(std::ostringstream &str, int timeZoneDifferential);
};
}
