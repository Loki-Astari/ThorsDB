
#include "gtest/gtest.h"
#include "test/TableTest.h"
//#include "ThorsDB/Endian.h"
#include <arpa/inet.h>
#include <cstdint>

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

// Bit Values can only be read into "unsigned integer types"
TEST(TableBitTest, ReadBitIntoUChar)      { typeGoodTest<unsigned char>  (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoUShort)     { typeGoodTest<unsigned short> (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoUInt)       { typeGoodTest<unsigned int>   (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoULong)      { typeGoodTest<unsigned long>  (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoULongLong)  { typeGoodTest<unsigned long long>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }

TEST(TableBitTest, ReadBitIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }

TEST(TableBitTest, ReadBitIntoString)     { typeBadTest<std::string,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoBlob)       { typeBadTest<std::vector<char>, std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoFloat)      { typeBadTest<float,           std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBitIntoDouble)     { typeBadTest<double,          std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }

// Can't read a 12 bit value into all int types
TEST(TableBitTest, ReadBit12IntoUChar)    { typeBadTest<unsigned char,   std::logic_error>("SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoUShort)   { typeGoodTest<unsigned short> (static_cast<unsigned short>(htons(0xB3D)), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoUInt)     { typeGoodTest<unsigned int>   (static_cast<unsigned int>(htons(0x0B3D)), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(htons(0x0B3D)), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit12IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(htons(0x0B3D)), "SELECT B1 FROM IntTypes where Id=1"); }

// Can't read a 24 bit value into all int types
TEST(TableBitTest, ReadBit24IntoUChar)    { typeBadTest<unsigned char,   std::logic_error>("SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoUShort)   { typeBadTest<unsigned short,  std::logic_error>("SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoUInt)     { typeGoodTest<unsigned int>   (static_cast<unsigned int>(htonl(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(htonl(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit24IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(htonl(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }

// Can't read a 48 bit value into all int types
TEST(TableBitTest, ReadBit48IntoUChar)    { typeBadTest<unsigned char,   std::logic_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit48IntoUShort)   { typeBadTest<unsigned short,  std::logic_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit48IntoUInt)     { typeBadTest<unsigned int,    std::logic_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit48IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(htonll(0xFEDCBA987654) >> 16), "SELECT B3 FROM IntTypes where Id=1"); }
TEST(TableBitTest, ReadBit48IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(htonll(0xFEDCBA987654) >> 16), "SELECT B3 FROM IntTypes where Id=1"); }


