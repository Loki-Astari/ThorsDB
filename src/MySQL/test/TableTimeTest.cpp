
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
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

using TTS = ThorsAnvil::DB::Access::UnixTimeStamp;
using namespace std::string_literals;

TTS getEpochFromTimeStamp(std::string const& timeStamp)
{
    struct tm tm;
    if (strptime(timeStamp.c_str(), "%Y-%m-%d %H:%M:%S", &tm) == NULL )
    {   throw std::runtime_error("Invalid Time Stamp");
    }
    return TTS(timegm(&tm));
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


