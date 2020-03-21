#pragma once

#include <string_view>

namespace datetime {
/// The date/time format defined in the ISO 8601 standard.
///
/// Examples: 
///   2005-01-01T12:00:00+01:00
///   2005-01-01T11:00:00Z
static constexpr std::string_view ISO8601_FORMAT = "%Y-%m-%dT%H:%M:%S%z";

/// The date/time format defined in the ISO 8601 standard,
/// with fractional seconds.
///
/// Examples: 
///   2005-01-01T12:00:00.000000+01:00
///   2005-01-01T11:00:00.000000Z
static constexpr std::string_view ISO8601_FRAC_FORMAT = "%Y-%m-%dT%H:%M:%s%z";

/// The date/time format defined in RFC 822 (obsoleted by RFC 1123).
///
/// Examples: 
///   Sat, 1 Jan 05 12:00:00 +0100
///   Sat, 1 Jan 05 11:00:00 GMT
static constexpr std::string_view RFC822_FORMAT = "%w, %e %b %y %H:%M:%S %Z";

/// The date/time format defined in RFC 1123 (obsoletes RFC 822).
///
/// Examples: 
///   Sat, 1 Jan 2005 12:00:00 +0100
///   Sat, 1 Jan 2005 11:00:00 GMT
static constexpr std::string_view RFC1123_FORMAT = "%w, %e %b %Y %H:%M:%S %Z";

/// The date/time format defined in the HTTP specification (RFC 2616),
/// which is basically a variant of RFC 1036 with a zero-padded day field.
///
/// Examples: 
///   Sat, 01 Jan 2005 12:00:00 +0100
///   Sat, 01 Jan 2005 11:00:00 GMT
static constexpr std::string_view HTTP_FORMAT = "%w, %d %b %Y %H:%M:%S %Z";

/// The date/time format defined in RFC 850 (obsoleted by RFC 1036).
///
/// Examples: 
///   Saturday, 1-Jan-05 12:00:00 +0100
///   Saturday, 1-Jan-05 11:00:00 GMT
static constexpr std::string_view RFC850_FORMAT = "%W, %e-%b-%y %H:%M:%S %Z";

/// The date/time format defined in RFC 1036 (obsoletes RFC 850).
///
/// Examples: 
///   Saturday, 1 Jan 05 12:00:00 +0100
///   Saturday, 1 Jan 05 11:00:00 GMT
static constexpr std::string_view RFC1036_FORMAT = "%W, %e %b %y %H:%M:%S %Z";

/// The date/time format produced by the ANSI C asctime() function.
///
/// Example: 
///   Sat Jan  1 12:00:00 2005
static constexpr std::string_view ASCTIME_FORMAT = "%w %b %f %H:%M:%S %Y";

/// A simple, sortable date/time format.
///
/// Example:
///   2005-01-01 12:00:00
static constexpr std::string_view SORTABLE_FORMAT = "%Y-%m-%d %H:%M:%S";
}
