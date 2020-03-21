#include "Timespan.hpp"

#include <algorithm>

namespace Poco {
const Timespan::TimeDiff Timespan::MILLISECONDS = 1000;
const Timespan::TimeDiff Timespan::SECONDS = 1000 * MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES = 60 * SECONDS;
const Timespan::TimeDiff Timespan::HOURS = 60 * MINUTES;
const Timespan::TimeDiff Timespan::DAYS = 24 * HOURS;

Timespan::Timespan() :
	_span(0) {
}

Timespan::Timespan(TimeDiff microseconds) :
	_span(microseconds) {
}

Timespan::Timespan(long seconds, long microseconds) :
	_span(TimeDiff(seconds) * SECONDS + microseconds) {
}

Timespan::Timespan(int days, int hours, int minutes, int seconds, int microseconds) :
	_span(TimeDiff(microseconds) + TimeDiff(seconds) * SECONDS + TimeDiff(minutes) * MINUTES + TimeDiff(hours) * HOURS + TimeDiff(days) * DAYS) {
}

Timespan::Timespan(const Timespan &timespan) :
	_span(timespan._span) {
}

int Timespan::GetDays() const {
	return int(_span / DAYS);
}

int Timespan::GetHours() const {
	return int((_span / HOURS) % 24);
}

int Timespan::GetTotalHours() const {
	return int(_span / HOURS);
}

int Timespan::GetMinutes() const {
	return int((_span / MINUTES) % 60);
}

int Timespan::GetTotalMinutes() const {
	return int(_span / MINUTES);
}

int Timespan::GetSeconds() const {
	return int((_span / SECONDS) % 60);
}

int Timespan::GetTotalSeconds() const {
	return int(_span / SECONDS);
}

int Timespan::GetMilliseconds() const {
	return int((_span / MILLISECONDS) % 1000);
}

Timespan::TimeDiff Timespan::GetTotalMilliseconds() const {
	return _span / MILLISECONDS;
}

int Timespan::GetMicroseconds() const {
	return int(_span % 1000);
}

int Timespan::GetNanoseconds() const {
	return int(_span % 1000000);
}

Timespan::TimeDiff Timespan::GetTotalMicroseconds() const {
	return _span;
}

bool Timespan::operator==(const Timespan &ts) const {
	return _span == ts._span;
}

bool Timespan::operator!=(const Timespan &ts) const {
	return _span != ts._span;
}

bool Timespan::operator>(const Timespan &ts) const {
	return _span > ts._span;
}

bool Timespan::operator>=(const Timespan &ts) const {
	return _span >= ts._span;
}

bool Timespan::operator<(const Timespan &ts) const {
	return _span < ts._span;
}

bool Timespan::operator<=(const Timespan &ts) const {
	return _span <= ts._span;
}

Timespan Timespan::operator+(const Timespan &d) const {
	return Timespan(_span + d._span);
}

Timespan Timespan::operator-(const Timespan &d) const {
	return Timespan(_span - d._span);
}

Timespan &Timespan::operator+=(const Timespan &d) {
	_span += d._span;
	return *this;
}

Timespan &Timespan::operator-=(const Timespan &d) {
	_span -= d._span;
	return *this;
}

Timespan &Timespan::operator=(TimeDiff microseconds) {
	_span = microseconds;
	return *this;
}

bool Timespan::operator==(TimeDiff microseconds) const {
	return _span == microseconds;
}

bool Timespan::operator!=(TimeDiff microseconds) const {
	return _span != microseconds;
}

bool Timespan::operator>(TimeDiff microseconds) const {
	return _span > microseconds;
}

bool Timespan::operator>=(TimeDiff microseconds) const {
	return _span >= microseconds;
}

bool Timespan::operator<(TimeDiff microseconds) const {
	return _span < microseconds;
}

bool Timespan::operator<=(TimeDiff microseconds) const {
	return _span <= microseconds;
}

Timespan Timespan::operator+(TimeDiff microseconds) const {
	return Timespan(_span + microseconds);
}

Timespan Timespan::operator-(TimeDiff microseconds) const {
	return Timespan(_span - microseconds);
}

Timespan &Timespan::operator+=(TimeDiff microseconds) {
	_span += microseconds;
	return *this;
}

Timespan &Timespan::operator-=(TimeDiff microseconds) {
	_span -= microseconds;
	return *this;
}
}
