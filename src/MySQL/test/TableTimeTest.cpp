
#include "gtest/gtest.h"
#include "test/TableTest.h"
#ifdef __WINNT__
#else
#include <arpa/inet.h>
#endif
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table TimeStampTypes
 *
 * MYSQL_TYPE_TIMESTAMP
 * MYSQL_TYPE_DATETIME
 * MYSQL_TYPE_TIMESTAMP2
 * MYSQL_TYPE_DATETIME2
 * MYSQL_TYPE_DATE
 * MYSQL_TYPE_TIME
 * MYSQL_TYPE_TIME2
 * MYSQL_TYPE_YEAR
 * MYSQL_TYPE_NEWDATE
 */

#ifdef __WINNT__
#include <time.h>
#include <iomanip>
#include <sstream>

extern "C" char* strptime(const char* s,
                          const char* f,
                          struct tm* tm) {
  // Isn't the C++ standard lib nice? std::get_time is defined such that its
  // format parameters are the exact same as strptime. Of course, we have to
  // create a string stream first, and imbue it with the current C locale, and
  // we also have to make sure we return the right things if it fails, or
  // if it succeeds, but this is still far simpler an implementation than any
  // of the versions in any of the C standard libraries.
  std::istringstream input(s);
  input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
  input >> std::get_time(tm, f);
  if (input.fail()) {
    return nullptr;
  }
  return (char*)(s + input.tellg());
}
#endif

using TTS = ThorsAnvil::DB::Access::UnixTimeStamp;
using namespace std::string_literals;

TTS getEpochFromTimeStamp(std::string const& timeStamp)
{
    struct tm tm;
    if (strptime(timeStamp.c_str(), "%Y-%m-%d %H:%M:%S", &tm) == NULL )
    {   throw std::runtime_error("Invalid Time Stamp");
    }
#ifdef __WINNT__
    return TTS(_mkgmtime(&tm));
#else
    return TTS(timegm(&tm));
#endif
}
TTS getEpochFromDate(std::string const& date)
{
    return getEpochFromTimeStamp(date + " 00:00:00");
}
TTS getEpochFromTime(std::string const& time)
{
    return getEpochFromTimeStamp("1970-01-01 "s + time);
}

// Bit Values can only be read into "unsigned integer types"
// 1969-04-25

TEST(TableTimeTest, ReadDate)
{
    typeGoodTest<TTS>(getEpochFromDate("1999-04-25"), "SELECT ST1 FROM TimeStampTypes WHERE Id=1");
}
TEST(TableTimeTest, ReadTime)
{
    typeGoodTest<TTS>        (getEpochFromTime("06:15:43"), "SELECT ST2 FROM TimeStampTypes WHERE Id=1");
}
TEST(TableTimeTest, ReadTS)
{
    typeGoodTest<TTS>(getEpochFromTimeStamp("1999-04-25 06:15:43"), "SELECT ST3 FROM TimeStampTypes WHERE Id=1");
}
TEST(TableTimeTest, ReadDateTime)
{
    typeGoodTest<TTS>(getEpochFromTimeStamp("1999-04-25 06:15:43"), "SELECT ST4 FROM TimeStampTypes WHERE Id=1");
}
TEST(TableTimeTest, ReadYear)
{
    typeGoodTest<int>(1999, "SELECT ST5 FROM TimeStampTypes WHERE Id=1");
}


