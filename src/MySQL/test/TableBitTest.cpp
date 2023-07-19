
#include "gtest/gtest.h"
#include "test/TableTest.h"
//#include "ThorsDB/Endian.h"
#ifdef __WINNT__
#else
#include <arpa/inet.h>
#endif
#include <cstdint>
#include <boost/endian/conversion.hpp>

/*
 * see test/data/init.sql
 * Add test data to this file in table IntTypes
 *
 * Bit Filed can go into:
 *
 * unsigned char
 * unsigned short
 * unsigned int
 * unsigned long
 * unsigned long long

 * Should be an error trying to put it in:
 *
 * Other int types
 * std::string
 * std::vector
 * float
 * double
 */

using boost::endian::native_to_big;

// Bit Values can only be read into "unsigned integer types"
TEST(TableBitTest, ReadBitIntoUChar)      { typeGoodTest<unsigned char>  (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoUShort)     { typeGoodTest<unsigned short> (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoUInt)       { typeGoodTest<unsigned int>   (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoULong)      { typeGoodTest<unsigned long>  (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoULongLong)  { typeGoodTest<unsigned long long>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }

TEST(TableBitTest, ReadBitIntoChar)       { typeBadTest<char,            ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSChar)      { typeBadTest<signed char,     ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSShort)     { typeBadTest<signed short,    ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSInt)       { typeBadTest<signed int,      ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSLong)      { typeBadTest<signed long,     ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSLongLong)  { typeBadTest<signed long long,ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }

TEST(TableBitTest, ReadBitIntoString)     { typeBadTest<std::string,     ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoBlob)       { typeBadTest<std::vector<char>, ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoFloat)      { typeBadTest<float,           ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoDouble)     { typeBadTest<double,          ThorsAnvil::Logging::LogicalException>("SELECT B0 FROM IntTypes where Id=1"); }

// Can't read a 12 bit value into all int types
TEST(TableBitTest, ReadBit12IntoUChar)    { typeBadTest<unsigned char,   ThorsAnvil::Logging::LogicalException>("SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoUShort)   { typeGoodTest<unsigned short> (static_cast<unsigned short>(native_to_big(0xB3D) >> 16), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoUInt)     { typeGoodTest<unsigned int>   (static_cast<unsigned int>(native_to_big(0x0B3D) >> 16), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(native_to_big(0x0B3D) >> 16), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(native_to_big(0x0B3D) >> 16), "SELECT B1 FROM IntTypes where Id=1"); }

// Can't read a 24 bit value into all int types
TEST(TableBitTest, ReadBit24IntoUChar)    { typeBadTest<unsigned char,   ThorsAnvil::Logging::LogicalException>("SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoUShort)   { typeBadTest<unsigned short,  ThorsAnvil::Logging::LogicalException>("SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoUInt)     { typeGoodTest<unsigned int>   (static_cast<unsigned int>(native_to_big(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(native_to_big(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(native_to_big(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }

// Can't read a 48 bit value into all int types
TEST(TableBitTest, ReadBit48IntoUChar)    { typeBadTest<unsigned char,   ThorsAnvil::Logging::LogicalException>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit48IntoUShort)   { typeBadTest<unsigned short,  ThorsAnvil::Logging::LogicalException>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit48IntoUInt)     { typeBadTest<unsigned int,    ThorsAnvil::Logging::LogicalException>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit48IntoULong)
{
    if (sizeof(unsigned long) < 48)
    {
        GTEST_SKIP() << "Unsigned long not large enough for test";
    }

    typeGoodTest<unsigned long>  (static_cast<unsigned long>(native_to_big(0xFEDCBA987654) >> 16), "SELECT B3 FROM IntTypes where Id=1");
}
TEST(TableBitTest, ReadBit48IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(native_to_big(0xFEDCBA987654) >> 16), "SELECT B3 FROM IntTypes where Id=1"); }


