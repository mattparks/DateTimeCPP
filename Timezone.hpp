#pragma once

#include <string>

#include "Timestamp.hpp"

namespace Poco {
/// This class provides information about the current timezone.
/// TODO: std::chrono::time_zone C++20
class Timezone {
public:
	/// Returns the offset of local time to UTC, in seconds.
	///     local time = UTC + utcOffset() + dst().
	static int UtcOffset();

	/// Returns the daylight saving time offset in seconds if
	/// daylight saving time is in use.
	///     local time = UTC + utcOffset() + dst().
	static int Dst();

	/// Returns true if daylight saving time is in effect
	/// for the given time. Depending on the operating system
	/// platform this might only work reliably for certain
	/// date ranges, as the C library's localtime() function
	/// is used.
	static bool IsDst(const Timestamp &timestamp);

	/// Returns the time zone differential for the current timezone.
	/// The timezone differential is computed as utcOffset() + dst()
	/// and is expressed in seconds.
	static int Tzd();

	/// Returns the timezone name currently in effect.
	static std::string Name();

	/// Returns the timezone name if not daylight saving time is in effect.
	static std::string StandardName();

	/// Returns the timezone name if daylight saving time is in effect.
	static std::string DstName();
};

inline int Timezone::Tzd() {
	return UtcOffset() + Dst();
}
}
