
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "gtest/gtest.h"
#include "MySQLConfig.h"
#include <arpa/inet.h>
#include <cstdint>

/*
unsigned long long
unsigned long
unsigned int
unsigned short
unsigned char

signed long long
signed long
signed int
signed short
signed char
char

std::string
std::vector<char>
float
double
*/
/*
htonll seems to be define on my machine.
Leaving this here for future reference.
inline std::uint64_t htonll(std::uint64_t value)
{
    uint64_t  result;
    uint32_t* inputValue    = reinterpret_cast<uint32_t*>(&value);
    uint32_t* outputValue   = reinterpret_cast<uint32_t*>(&result);

    outputValue[0] = htonl(inputValue[1]);
    outputValue[1] = htonl(inputValue[0]);
    return(result);
}
*/

template<typename T>
void typeGoodTest(T expected, std::string const& expr)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, expr);
    statement.execute([&expected](T const& select){
        ASSERT_EQ(expected, select);
    });
}
template<typename T, typename ExceptionType>
void typeBadTest(std::string const& expr)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, expr);
    ASSERT_THROW(
        statement.execute([](T const& select){
            ASSERT_TRUE(false); // Should not reach here.
        }),
        ExceptionType
    );
}

// Bit Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadBitIntoUChar)      { typeGoodTest<unsigned char>  (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoUShort)     { typeGoodTest<unsigned short> (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoUInt)       { typeGoodTest<unsigned int>   (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoULong)      { typeGoodTest<unsigned long>  (0xA, "SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoULongLong)  { typeGoodTest<unsigned long long>(0xA, "SELECT B0 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadBitIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadBitIntoString)     { typeBadTest<std::string,     std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoBlob)       { typeBadTest<std::vector<char>, std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoFloat)      { typeBadTest<float,           std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBitIntoDouble)     { typeBadTest<double,          std::logic_error>("SELECT B0 FROM IntTypes where Id=1"); }

// Can't read a 12 bit value into all int types
TEST(intTableTest, ReadBit12IntoUChar)    { typeBadTest<unsigned char,   std::runtime_error>("SELECT B1 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit12IntoUShort)   { typeGoodTest<unsigned short> (static_cast<unsigned short>(htons(0xB3D)), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit12IntoUInt)     { typeGoodTest<unsigned int>   (static_cast<unsigned int>(htons(0x0B3D)), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit12IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(htons(0x0B3D)), "SELECT B1 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit12IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(htons(0x0B3D)), "SELECT B1 FROM IntTypes where Id=1"); }

// Can't read a 24 bit value into all int types
TEST(intTableTest, ReadBit24IntoUChar)    { typeBadTest<unsigned char,   std::runtime_error>("SELECT B2 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit24IntoUShort)   { typeBadTest<unsigned short,  std::runtime_error>("SELECT B2 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit24IntoUInt)     { typeGoodTest<unsigned int>   (static_cast<unsigned int>(htonl(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit24IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(htonl(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit24IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(htonl(0x123456) >> 8), "SELECT B2 FROM IntTypes where Id=1"); }

// Can't read a 48 bit value into all int types
TEST(intTableTest, ReadBit48IntoUChar)    { typeBadTest<unsigned char,   std::runtime_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit48IntoUShort)   { typeBadTest<unsigned short,  std::runtime_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit48IntoUInt)     { typeBadTest<unsigned int,    std::runtime_error>("SELECT B3 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit48IntoULong)    { typeGoodTest<unsigned long>  (static_cast<unsigned long>(htonll(0xFEDCBA987654) >> 16), "SELECT B3 FROM IntTypes where Id=1"); }
TEST(intTableTest, ReadBit48IntoULongLong){ typeGoodTest<unsigned long long>(static_cast<unsigned long long>(htonll(0xFEDCBA987654) >> 16), "SELECT B3 FROM IntTypes where Id=1"); }

// Unsigned Tiny (0->255) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadUTinyIntoUChar)     { typeGoodTest<unsigned char> (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoUShort)    { typeGoodTest<unsigned short>(0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoUInt)      { typeGoodTest<unsigned int>  (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoULong)     { typeGoodTest<unsigned long> (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoULongLong) { typeGoodTest<unsigned long long>(0x12, "SELECT U2 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadUTinyIntoChar)      { typeGoodTest<char>          (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoSChar)     { typeBadTest<signed char,    std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoSShort)    { typeBadTest<signed short,   std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoSInt)      { typeBadTest<signed int,     std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoSLong)     { typeBadTest<signed long,    std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUTinyIntoSLongLong) { typeBadTest<signed long long,std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }

// Tiny (-128->127) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadTinyIntoUChar)      { typeBadTest<unsigned char,  std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoUShort)     { typeBadTest<unsigned short, std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoUInt)       { typeBadTest<unsigned int,   std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoULong)      { typeBadTest<unsigned long,  std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoULongLong)  { typeBadTest<unsigned long long,std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadTinyIntoChar)       { typeGoodTest<char>          (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoSChar)      { typeGoodTest<signed char>   (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoSShort)     { typeGoodTest<signed short>  (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoSInt)       { typeGoodTest<signed int>    (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoSLong)      { typeGoodTest<signed long>   (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadTinyIntoSLongLong)  { typeGoodTest<signed long long>(0x12, "SELECT I2 FROM IntTypes where Id=1"); }

// Unsigned Small (0->65536) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadUSmallIntoUChar)    { typeBadTest<unsigned char,  std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoUShort)   { typeGoodTest<unsigned short>(0x34, "SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoUInt)     { typeGoodTest<unsigned int>  (0x34, "SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoULong)    { typeGoodTest<unsigned long> (0x34, "SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoULongLong){ typeGoodTest<unsigned long long>(0x34, "SELECT U3 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadUSmallIntoChar)     { typeBadTest<char,           std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoSChar)    { typeBadTest<signed char,    std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoSShort)   { typeBadTest<signed short,   std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoSInt)     { typeBadTest<signed int,     std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoSLong)    { typeBadTest<signed long,    std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUSmallIntoSLongLong){ typeBadTest<signed long long,std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }

// Small (-32768->32767) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadSmallIntoUChar)     { typeBadTest<unsigned char,   std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoUShort)    { typeBadTest<unsigned short,  std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoUInt)      { typeBadTest<unsigned int,    std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoULong)     { typeBadTest<unsigned long,   std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoULongLong) { typeBadTest<unsigned long long,std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadSmallIntoChar)      { typeBadTest<char,            std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoSChar)     { typeBadTest<signed char,     std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoSShort)    { typeGoodTest<signed short>   (0x34, "SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoSInt)      { typeGoodTest<signed int>     (0x34, "SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoSLong)     { typeGoodTest<signed long>    (0x34, "SELECT I3 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadSmallIntoSLongLong) { typeGoodTest<signed long long>(0x34, "SELECT I3 FROM IntTypes where Id=1"); }

// Unsigned Med (0->16777215) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadUMedIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoUInt)       { typeGoodTest<unsigned int>   (0x56, "SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoULong)      { typeGoodTest<unsigned long>  (0x56, "SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoULongLong)  { typeGoodTest<unsigned long long>(0x56, "SELECT U4 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadUMedIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest8, ReadUMedIntoSInt)      { typeBadTest<signed int,      std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUMedIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }

// Med (-838860->8388607) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadMedIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadMedIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoSInt)        { typeGoodTest<signed int>     (0x56, "SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoSLong)       { typeGoodTest<signed long>    (0x56, "SELECT I4 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadMedIntoSLongLong)   { typeGoodTest<signed long long>(0x56, "SELECT I4 FROM IntTypes where Id=1"); }

// Unsigned Int (0->4294967295) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadUIntIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoUInt)       { typeGoodTest<unsigned int>   (0x78, "SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoULong)      { typeGoodTest<unsigned long>  (0x78, "SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoULongLong)  { typeGoodTest<unsigned long long>(0x78, "SELECT U5 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadUIntIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUIntIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }

// Int (-2147483648->2147483647) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadIntIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadIntIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoSInt)        { typeGoodTest<signed int>     (0x78, "SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoSLong)       { typeGoodTest<signed long>    (0x78, "SELECT I5 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadIntIntoSLongLong)   { typeGoodTest<signed long long>(0x78, "SELECT I5 FROM IntTypes where Id=1"); }

// Unsigned Nor (0->4294967295) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadUNorIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoUInt)       { typeGoodTest<unsigned int>   (0x9A, "SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoULong)      { typeGoodTest<unsigned long>  (0x9A, "SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoULongLong)  { typeGoodTest<unsigned long long>(0x9A, "SELECT U6 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadUNorIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUNorIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }

// Nor (-2147483648->2147483647) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadNorIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadNorIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoSInt)        { typeGoodTest<signed int>     (0x9A, "SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoSLong)       { typeGoodTest<signed long>    (0x9A, "SELECT I6 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadNorIntoSLongLong)   { typeGoodTest<signed long long>(0x9A, "SELECT I6 FROM IntTypes where Id=1"); }

// Unsigned Big (0->18446744073709551615) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadUBigIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoUInt)       { typeBadTest<unsigned int,    std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoULong)      { typeGoodTest<unsigned long>  (0xCD, "SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoULongLong)  { typeGoodTest<unsigned long long>(0xCD, "SELECT U7 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadUBigIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadUBigIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }

// Big (-9223372036854775808->9223372036854775807) Values can only be read into "unsigned integer types"
TEST(IntTableTest, ReadBigIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }

TEST(IntTableTest, ReadBigIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoSInt)        { typeBadTest<signed int,      std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoSLong)       { typeGoodTest<signed long>    (0xCD, "SELECT I7 FROM IntTypes where Id=1"); }
TEST(IntTableTest, ReadBigIntoSLongLong)   { typeGoodTest<signed long long>(0xCD, "SELECT I7 FROM IntTypes where Id=1"); }

// Test MIN values.
// Only doing signed values.
// -128, -32768, -8388608, -2147483648, -2147483648, -9223372036854775808);
TEST(IntTableTest, ReadTinyCharMin)        { typeGoodTest<char>           (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadTinyShortMin)       { typeGoodTest<short>          (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadTinyIngMin)         { typeGoodTest<int>            (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadTinyLongMin)        { typeGoodTest<long>           (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadTinyLongLongMin)    { typeGoodTest<long long>      (-128, "SELECT I2 FROM IntTypes Where Id=2");}

TEST(IntTableTest, ReadSmallShortMin)      { typeGoodTest<short>          (-32768, "SELECT I3 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadSmallIntMin)        { typeGoodTest<int>            (-32768, "SELECT I3 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadSmallLongMin)       { typeGoodTest<long>           (-32768, "SELECT I3 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadSmallLongLongMin)   { typeGoodTest<long long>      (-32768, "SELECT I3 FROM IntTypes Where Id=2");}

TEST(IntTableTest, ReadMedIntMin)          { typeGoodTest<int>            (-8388608, "SELECT I4 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadMedLongMin)         { typeGoodTest<long>           (-8388608, "SELECT I4 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadMedLongLongMin)     { typeGoodTest<long long>      (-8388608, "SELECT I4 FROM IntTypes Where Id=2");}

TEST(IntTableTest, ReadIntLongMin)         { typeGoodTest<long>           (-2147483648, "SELECT I5 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadIntLongLongMin)     { typeGoodTest<long long>      (-2147483648, "SELECT I5 FROM IntTypes Where Id=2");}

TEST(IntTableTest, ReadNorLongMin)         { typeGoodTest<long>           (-2147483648, "SELECT I6 FROM IntTypes Where Id=2");}
TEST(IntTableTest, ReadNorLongLongMin)     { typeGoodTest<long long>      (-2147483648, "SELECT I6 FROM IntTypes Where Id=2");}

TEST(IntTableTest, ReadBigLongLongMin)     { typeGoodTest<long long>      (-9223372036854775808ULL, "SELECT I7 FROM IntTypes Where Id=2");}

// Test MAX values
// The signed values
// 127, 32767, 8388607, 2147483647, 2147483647, 9223372036854775807);
TEST(IntTableTest, ReadTinyCharMax)        { typeGoodTest<char>           (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyShortMax)       { typeGoodTest<short>          (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyIntMax)         { typeGoodTest<int>            (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyLongMax)        { typeGoodTest<long>           (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyLongLongMax)    { typeGoodTest<long long>      (127, "SELECT I2 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadSmallShortMax)      { typeGoodTest<short>          (32767, "SELECT I3 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadSmallIntMax)        { typeGoodTest<int>            (32767, "SELECT I3 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadSmallLongMax)       { typeGoodTest<long>           (32767, "SELECT I3 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadSmallLongLongMax)   { typeGoodTest<long long>      (32767, "SELECT I3 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadMedIntMax)          { typeGoodTest<int>            (8388607, "SELECT I4 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadMedLongMax)         { typeGoodTest<long>           (8388607, "SELECT I4 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadMedLongLongMax)     { typeGoodTest<long long>      (8388607, "SELECT I4 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadIntLongMax)         { typeGoodTest<long>           (2147483647, "SELECT I5 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadIntLongLongMax)     { typeGoodTest<long long>      (2147483647, "SELECT I5 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadNorLongMax)         { typeGoodTest<long>           (2147483647, "SELECT I6 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadNorLongLongMax)     { typeGoodTest<long long>      (2147483647, "SELECT I6 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadBigLongLongMax)     { typeGoodTest<long long>      (9223372036854775807ULL, "SELECT I7 FROM IntTypes Where Id=3");}

// Test MAX unsigned values
// The signed values
// 255, 65535, 16777215, 4294967295, 4294967295, 18446744073709551615,
TEST(IntTableTest, ReadTinyUCharMax)       { typeGoodTest<unsigned char>  (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyUShortMax)      { typeGoodTest<unsigned short> (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyUIntMax)        { typeGoodTest<unsigned int>   (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyULongMax)       { typeGoodTest<unsigned long>  (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadTinyULongLongMax)   { typeGoodTest<unsigned long long>(255, "SELECT U2 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadSmallUShortMax)     { typeGoodTest<unsigned short> (65535, "SELECT U3 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadSmallUIntMax)       { typeGoodTest<unsigned int>   (65535, "SELECT U3 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadSmallULongMax)      { typeGoodTest<unsigned long>  (65535, "SELECT U3 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadSmallULongLongMax)  { typeGoodTest<unsigned long long>(65535, "SELECT U3 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadMedUIntMax)         { typeGoodTest<unsigned int>   (16777215, "SELECT U4 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadMedULongMax)        { typeGoodTest<unsigned long>  (16777215, "SELECT U4 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadMedULongLongMax)    { typeGoodTest<unsigned long long>(16777215, "SELECT U4 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadIntULongMax)        { typeGoodTest<unsigned long>  (4294967295, "SELECT U5 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadIntULongLongMax)    { typeGoodTest<unsigned long long>(4294967295, "SELECT U5 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadNorULongMax)        { typeGoodTest<unsigned long>  (4294967295, "SELECT U6 FROM IntTypes Where Id=3");}
TEST(IntTableTest, ReadNorULongLongMax)    { typeGoodTest<unsigned long long>(4294967295, "SELECT U6 FROM IntTypes Where Id=3");}

TEST(IntTableTest, ReadBigULongLongMax)    { typeGoodTest<unsigned long long>(18446744073709551615ULL, "SELECT U7 FROM IntTypes Where Id=3");}


TEST(intTableTest,ALL)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "SELECT * from IntTypes");
}

