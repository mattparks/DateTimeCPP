#if defined(__UNIX__)
#include "Timezone.hpp"

#include <stdexcept>
#include <ctime>

namespace Poco {
class TZInfo {
public:
	TZInfo() {
		tzset();
	}

	int TimeZone() {
#if defined(__APPLE__)  || defined(__FreeBSD__) || defined(ANDROID) // no timezone global var
		std::time_t now = std::time(NULL);
		struct std::tm t;
		gmtime_r(&now, &t);
		std::time_t utc = std::mktime(&t);
		return now - utc;
#elif defined(__CYGWIN__)
		return -_timezone;
#else
		return -timezone;
#endif
	}

	const char *Name(bool dst) {
		return tzname[dst ? 1 : 0];
	}
};

static TZInfo tzInfo;

int Timezone::UtcOffset() {
	return tzInfo.TimeZone();
}

int Timezone::Dst() {
	std::time_t now = std::time(NULL);
	struct std::tm t;
	if (!localtime_r(&now, &t))
		throw std::runtime_error("cannot get local time DST offset");
	return t.tm_isdst == 1 ? 3600 : 0;
}

bool Timezone::IsDst(const Timestamp &timestamp) {
	std::time_t time = timestamp.EpochTime();
	struct std::tm *tms = std::localtime(&time);
	if (!tms) throw std::runtime_error("cannot get local time DST flag");
	return tms->tm_isdst > 0;
}

std::string Timezone::Name() {
	return std::string(tzInfo.Name(dst() != 0));
}

std::string Timezone::StandardName() {
	return std::string(tzInfo.Name(false));
}

std::string Timezone::DstName() {
	return std::string(tzInfo.Name(true));
}
} 
#endif
