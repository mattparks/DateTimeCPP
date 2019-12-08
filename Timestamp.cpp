#include "Timestamp.h"

#include <chrono>
#include <limits>

#include "Timespan.h"

namespace Poco {
const Timestamp::TimeVal Timestamp::Min = std::numeric_limits<TimeVal>::min();
const Timestamp::TimeVal Timestamp::Max = std::numeric_limits<TimeVal>::max();

Timestamp::Timestamp() {
	Update();
}

Timestamp::Timestamp(TimeVal tv) {
	_ts = tv;
}

Timestamp::Timestamp(const Timestamp &other) {
	_ts = other._ts;
}

void Timestamp::Update() {
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	_ts = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

std::time_t Timestamp::GetEpochTime() const {
	return std::time_t(_ts / GetResolution());
}

Timestamp::UtcTimeVal Timestamp::GetUtcTime() const {
	return _ts * 10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}

Timestamp::TimeVal Timestamp::GetEpochMicroseconds() const {
	return _ts;
}

Timestamp::TimeDiff Timestamp::GetElapsed() const {
	Timestamp now;
	return now - *this;
}

bool Timestamp::IsElapsed(TimeDiff interval) const {
	Timestamp now;
	TimeDiff diff = now - *this;
	return diff >= interval;
}

Timestamp &Timestamp::operator=(TimeVal tv) {
	_ts = tv;
	return *this;
}

bool Timestamp::operator==(const Timestamp &ts) const {
	return _ts == ts._ts;
}

bool Timestamp::operator!=(const Timestamp &ts) const {
	return _ts != ts._ts;
}

bool Timestamp::operator>(const Timestamp &ts) const {
	return _ts > ts._ts;
}

bool Timestamp::operator>=(const Timestamp &ts) const {
	return _ts >= ts._ts;
}

bool Timestamp::operator<(const Timestamp &ts) const {
	return _ts < ts._ts;
}

bool Timestamp::operator<=(const Timestamp &ts) const {
	return _ts <= ts._ts;
}

Timestamp Timestamp::operator+(TimeDiff d) const {
	return Timestamp(_ts + d);
}

Timestamp Timestamp::operator+(const Timespan &span) const {
	return *this + span.GetTotalMicroseconds();
}

Timestamp Timestamp::operator-(TimeDiff d) const {
	return Timestamp(_ts - d);
}

Timestamp Timestamp::operator-(const Timespan &span) const {
	return *this - span.GetTotalMicroseconds();
}

Timestamp::TimeDiff Timestamp::operator-(const Timestamp &ts) const {
	return _ts - ts._ts;
}

Timestamp &Timestamp::operator+=(TimeDiff d) {
	_ts += d;
	return *this;
}

Timestamp & Timestamp::operator+=(const Timespan &span) {
	return *this += span.GetTotalMicroseconds();
}

Timestamp &Timestamp::operator-=(TimeDiff d) {
	_ts -= d;
	return *this;
}

Timestamp & Timestamp::operator-=(const Timespan &span) {
	return *this -= span.GetTotalMicroseconds();
}

Timestamp Timestamp::FromEpochTime(std::time_t t) {
	return Timestamp(TimeVal(t) * GetResolution());
}

Timestamp Timestamp::FromUtcTime(UtcTimeVal val) {
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return Timestamp(val);
}

Timestamp::TimeVal Timestamp::GetResolution() {
	return 1000000;
}
}
