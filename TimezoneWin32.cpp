#if defined(_WIN32)
#include "Timezone.hpp"

#include <stdexcept>
#include <ctime>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace Poco {
int Timezone::UtcOffset() {
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	return -tzInfo.Bias * 60;
}

int Timezone::Dst() {
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	return dstFlag == TIME_ZONE_ID_DAYLIGHT ? -tzInfo.DaylightBias * 60 : 0;
}

bool Timezone::IsDst(const Timestamp &timestamp) {
	std::time_t time = timestamp.GetEpochTime();
	struct std::tm *tms = std::localtime(&time);
	if (!tms) throw std::runtime_error("cannot get local time DST flag");
	return tms->tm_isdst > 0;
}

std::string Timezone::Name() {
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR *ptr = dstFlag == TIME_ZONE_ID_DAYLIGHT ? tzInfo.DaylightName : tzInfo.StandardName;
	char buffer[256];
	DWORD rc = WideCharToMultiByte(CP_ACP, 0, ptr, -1, buffer, sizeof(buffer), nullptr, nullptr);
	if (rc) result = buffer;
	return result;
}

std::string Timezone::StandardName() {
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR *ptr = tzInfo.StandardName;
	char buffer[256];
	DWORD rc = WideCharToMultiByte(CP_ACP, 0, ptr, -1, buffer, sizeof(buffer), nullptr, nullptr);
	if (rc) result = buffer;
	return result;
}

std::string Timezone::DstName() {
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR *ptr = tzInfo.DaylightName;
	char buffer[256];
	DWORD rc = WideCharToMultiByte(CP_ACP, 0, ptr, -1, buffer, sizeof(buffer), nullptr, nullptr);
	if (rc) result = buffer;
	return result;
}
}
#endif
