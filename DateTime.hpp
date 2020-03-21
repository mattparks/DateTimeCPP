#pragma once

#include <optional>
#include <date/tz.h>

#include "Date.hpp"
#include "TimeDelta.hpp"
#include "DateFormats.hpp"

namespace datetime {
template<class Duration = std::chrono::system_clock::duration>
class DateTime {
	using CommonDuration = typename std::common_type<Duration, std::chrono::seconds>::type;
public:
	//enum class FormatType {
	//	Strp, Java
	//};
	
	static DateTime<CommonDuration> Today();
	static DateTime<CommonDuration> Now(const std::string &timezoneName = "");
	static DateTime<CommonDuration> UtcNow();

	template<class Rep>
	static DateTime<CommonDuration> FromTimestamp(Rep timestamp, const std::string &timezoneName = "");
	template<class Rep>
	static DateTime<CommonDuration> UtcFromTimestamp(Rep timestamp);

	static DateTime<CommonDuration> Parse(const std::string &dateString, const std::string &format);
	static bool TryParse(const std::string &dateString, const std::string &format, DateTime<CommonDuration> &dateTime);
	static std::optional<DateTime<CommonDuration>> TryParse(const std::string &dateString, const std::string &format);

	DateTime() = default;
	DateTime(const date::zoned_time<CommonDuration> &zt) :
		_zt(zt) {
	}

	const date::zoned_time<CommonDuration> &ZonedTime() const { return _zt; }

	Date Date() const;
	date::year Year() const;
	date::month Month() const;
	date::day Day() const;

	const date::time_zone *Timezone() const;
	const std::string &TzInfo() const;

	TimeDelta UtcOffset() const;

	std::string Timestamp() const;

	std::string Format(std::string_view format = ISO8601_FORMAT) const;

private:
	date::fields<CommonDuration> FieldsYmdTime() const;

	date::zoned_time<CommonDuration> _zt;
};
}

#include "DateTime.inl"