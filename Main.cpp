#include <iostream>

#include <cassert>
#include "DateTime.hpp"

using namespace std::chrono_literals;
using namespace date::literals;
using namespace datetime;

int main() {
	auto x1 = Date::Today();
	std::cout << "Today's date is " << x1 << std::endl;

	auto x2 = DateTime<>::Now(); // same result as DateTime<>::today()
	std::cout << "Today's DateTime is " << x2 << std::endl;

	auto timeDelta = TimeDelta(date::weeks(1), date::days(2), std::chrono::hours(3), std::chrono::seconds(240), std::chrono::microseconds(10000)); // arguments order does not matter
	std::cout << "Here the delta time we want to add " << timeDelta << std::endl;

	std::cout << "Here the added result " << x1 + timeDelta << std::endl;

	// The C++ version can allow seconds only precision and can directly take a timezone string
	auto x3 = DateTime<std::chrono::seconds>::Now("Asia/Tehran");
	std::cout << "Time in " << x3.TzInfo() << " is " << x3 << std::endl;

	auto x4 = DateTime<>::Parse("21/11/92 16:30", "%d/%m/%y %H:%M");
	std::cout << "Parsed DateTime is " << x4 << std::endl;

	if (auto x5 = DateTime<>::TryParse("This is not a date!", "%d/%m/%y %H:%M"))
		std::cout << "datime from trystrptime is " << *x5 << std::endl;

	auto x6 = DateTime<>::Now();
	std::cout << "Formatted using STRP " << x6.Format("%d%b%Y %H-%M-%S.%F") << std::endl;
	std::cout << "Formatted using Java " << x6.Format("ddMMMyyyy HH-mm-ss.fffffff") << std::endl;

	if (x4 > x3 && x4 < x6)
		std::cout << "Newer date is newer" << std::endl;

	//auto x5 = DateTime<>::UtcFromTimestamp(1497252490.0282006);
	//std::cout << "UTC datetime from timestamp is " << x5 << ". Check timestamp back: " << x5.Timestamp() << std::endl;

	//std::cout << "You can substract two datetimes into a timedelta " << x5 - x4 << std::endl;
	return 0;
}
