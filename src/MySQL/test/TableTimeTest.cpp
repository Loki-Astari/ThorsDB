
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

using TTS = ThorsAnvil::SQL::UnixTimeStamp;

TTS getEpochTime(std::string const& timeStamp)
{
    struct tm tm;
    if (strptime(timeStamp.c_str(), "%Y-%m-%d %H:%M:%S", &tm) == NULL )
    {   throw std::runtime_error("Invalid Time Stamp");
    }
    return TTS(timegm(&tm));
}

// Bit Values can only be read into "unsigned integer types"
// 1969-04-25

TEST(TableTimeTest, ReateDate)        {typeGoodTest<TTS>        (getEpochTime("1969-04-25 00:00:00"), "SELECT ST1 FROM TimeStampTypes WHERE Id=1");}


