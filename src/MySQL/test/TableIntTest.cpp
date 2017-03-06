
#include "gtest/gtest.h"
#include "test/TableTest.h"
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

Special case char is both signed and unsigned.
char

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

// Unsigned Tiny (0->255) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadUTinyIntoUChar)     { typeGoodTest<unsigned char> (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoUShort)    { typeGoodTest<unsigned short>(0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoUInt)      { typeGoodTest<unsigned int>  (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoULong)     { typeGoodTest<unsigned long> (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoULongLong) { typeGoodTest<unsigned long long>(0x12, "SELECT U2 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadUTinyIntoChar)      { typeGoodTest<char>          (0x12, "SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoSChar)     { typeBadTest<signed char,    std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoSShort)    { typeBadTest<signed short,   std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoSInt)      { typeBadTest<signed int,     std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoSLong)     { typeBadTest<signed long,    std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUTinyIntoSLongLong) { typeBadTest<signed long long,std::logic_error>("SELECT U2 FROM IntTypes where Id=1"); }

// Tiny (-128->127) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadTinyIntoUChar)      { typeBadTest<unsigned char,  std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoUShort)     { typeBadTest<unsigned short, std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoUInt)       { typeBadTest<unsigned int,   std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoULong)      { typeBadTest<unsigned long,  std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoULongLong)  { typeBadTest<unsigned long long,std::logic_error>("SELECT I2 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadTinyIntoChar)       { typeGoodTest<char>          (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoSChar)      { typeGoodTest<signed char>   (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoSShort)     { typeGoodTest<signed short>  (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoSInt)       { typeGoodTest<signed int>    (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoSLong)      { typeGoodTest<signed long>   (0x12, "SELECT I2 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadTinyIntoSLongLong)  { typeGoodTest<signed long long>(0x12, "SELECT I2 FROM IntTypes where Id=1"); }

// Unsigned Small (0->65536) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadUSmallIntoUChar)    { typeBadTest<unsigned char,  std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoUShort)   { typeGoodTest<unsigned short>(0x34, "SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoUInt)     { typeGoodTest<unsigned int>  (0x34, "SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoULong)    { typeGoodTest<unsigned long> (0x34, "SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoULongLong){ typeGoodTest<unsigned long long>(0x34, "SELECT U3 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadUSmallIntoChar)     { typeBadTest<char,           std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoSChar)    { typeBadTest<signed char,    std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoSShort)   { typeBadTest<signed short,   std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoSInt)     { typeBadTest<signed int,     std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoSLong)    { typeBadTest<signed long,    std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUSmallIntoSLongLong){ typeBadTest<signed long long,std::logic_error>("SELECT U3 FROM IntTypes where Id=1"); }

// Small (-32768->32767) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadSmallIntoUChar)     { typeBadTest<unsigned char,   std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoUShort)    { typeBadTest<unsigned short,  std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoUInt)      { typeBadTest<unsigned int,    std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoULong)     { typeBadTest<unsigned long,   std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoULongLong) { typeBadTest<unsigned long long,std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadSmallIntoChar)      { typeBadTest<char,            std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoSChar)     { typeBadTest<signed char,     std::logic_error>("SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoSShort)    { typeGoodTest<signed short>   (0x34, "SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoSInt)      { typeGoodTest<signed int>     (0x34, "SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoSLong)     { typeGoodTest<signed long>    (0x34, "SELECT I3 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadSmallIntoSLongLong) { typeGoodTest<signed long long>(0x34, "SELECT I3 FROM IntTypes where Id=1"); }

// Unsigned Med (0->16777215) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadUMedIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoUInt)       { typeGoodTest<unsigned int>   (0x56, "SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoULong)      { typeGoodTest<unsigned long>  (0x56, "SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoULongLong)  { typeGoodTest<unsigned long long>(0x56, "SELECT U4 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadUMedIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest8, ReadUMedIntoSInt)      { typeBadTest<signed int,      std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUMedIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U4 FROM IntTypes where Id=1"); }

// Med (-838860->8388607) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadMedIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadMedIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoSInt)        { typeGoodTest<signed int>     (0x56, "SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoSLong)       { typeGoodTest<signed long>    (0x56, "SELECT I4 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadMedIntoSLongLong)   { typeGoodTest<signed long long>(0x56, "SELECT I4 FROM IntTypes where Id=1"); }

// Unsigned Int (0->4294967295) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadUIntIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoUInt)       { typeGoodTest<unsigned int>   (0x78, "SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoULong)      { typeGoodTest<unsigned long>  (0x78, "SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoULongLong)  { typeGoodTest<unsigned long long>(0x78, "SELECT U5 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadUIntIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUIntIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U5 FROM IntTypes where Id=1"); }

// Int (-2147483648->2147483647) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadIntIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadIntIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoSInt)        { typeGoodTest<signed int>     (0x78, "SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoSLong)       { typeGoodTest<signed long>    (0x78, "SELECT I5 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadIntIntoSLongLong)   { typeGoodTest<signed long long>(0x78, "SELECT I5 FROM IntTypes where Id=1"); }

// Unsigned Nor (0->4294967295) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadUNorIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoUInt)       { typeGoodTest<unsigned int>   (0x9A, "SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoULong)      { typeGoodTest<unsigned long>  (0x9A, "SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoULongLong)  { typeGoodTest<unsigned long long>(0x9A, "SELECT U6 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadUNorIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUNorIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U6 FROM IntTypes where Id=1"); }

// Nor (-2147483648->2147483647) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadNorIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadNorIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoSInt)        { typeGoodTest<signed int>     (0x9A, "SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoSLong)       { typeGoodTest<signed long>    (0x9A, "SELECT I6 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadNorIntoSLongLong)   { typeGoodTest<signed long long>(0x9A, "SELECT I6 FROM IntTypes where Id=1"); }

// Unsigned Big (0->18446744073709551615) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadUBigIntoUChar)      { typeBadTest<unsigned char,   std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoUShort)     { typeBadTest<unsigned short,  std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoUInt)       { typeBadTest<unsigned int,    std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoULong)      { typeGoodTest<unsigned long>  (0xCD, "SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoULongLong)  { typeGoodTest<unsigned long long>(0xCD, "SELECT U7 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadUBigIntoChar)       { typeBadTest<char,            std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoSChar)      { typeBadTest<signed char,     std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoSShort)     { typeBadTest<signed short,    std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoSInt)       { typeBadTest<signed int,      std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoSLong)      { typeBadTest<signed long,     std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadUBigIntoSLongLong)  { typeBadTest<signed long long,std::logic_error>("SELECT U7 FROM IntTypes where Id=1"); }

// Big (-9223372036854775808->9223372036854775807) Values can only be read into "unsigned integer types"
TEST(TableIntTest, ReadBigIntoUChar)       { typeBadTest<unsigned char,   std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoUShort)      { typeBadTest<unsigned short,  std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoUInt)        { typeBadTest<unsigned int,    std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoULong)       { typeBadTest<unsigned long,   std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoULongLong)   { typeBadTest<unsigned long long,std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }

TEST(TableIntTest, ReadBigIntoChar)        { typeBadTest<char,            std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoSChar)       { typeBadTest<signed char,     std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoSShort)      { typeBadTest<signed short,    std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoSInt)        { typeBadTest<signed int,      std::logic_error>("SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoSLong)       { typeGoodTest<signed long>    (0xCD, "SELECT I7 FROM IntTypes where Id=1"); }
TEST(TableIntTest, ReadBigIntoSLongLong)   { typeGoodTest<signed long long>(0xCD, "SELECT I7 FROM IntTypes where Id=1"); }

// Test MIN values.
// Only doing signed values.
// -128, -32768, -8388608, -2147483648, -2147483648, -9223372036854775808);
TEST(TableIntTest, ReadTinyCharMin)        { typeGoodTest<char>           (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadTinySCharMin)       { typeGoodTest<signed char>    (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadTinySShortMin)      { typeGoodTest<signed short>   (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadTinyISngMin)        { typeGoodTest<signed int>     (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadTinyLSongMin)       { typeGoodTest<signed long>    (-128, "SELECT I2 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadTinyLSongLongMin)   { typeGoodTest<signed long long>(-128, "SELECT I2 FROM IntTypes Where Id=2");}

TEST(TableIntTest, ReadSmallSShortMin)     { typeGoodTest<signed short>   (-32768, "SELECT I3 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadSmallISntMin)       { typeGoodTest<signed int>     (-32768, "SELECT I3 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadSmallLSongMin)      { typeGoodTest<signed long>    (-32768, "SELECT I3 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadSmallLSongLongMin)  { typeGoodTest<signed long long>(-32768, "SELECT I3 FROM IntTypes Where Id=2");}

TEST(TableIntTest, ReadMedISntMin)         { typeGoodTest<signed int>     (-8388608, "SELECT I4 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadMedLSongMin)        { typeGoodTest<signed long>    (-8388608, "SELECT I4 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadMedLSongLongMin)    { typeGoodTest<signed long long>(-8388608, "SELECT I4 FROM IntTypes Where Id=2");}

TEST(TableIntTest, ReadIntLSongMin)        { typeGoodTest<signed long>    (-2147483648, "SELECT I5 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadIntLSongLongMin)    { typeGoodTest<signed long long>(-2147483648, "SELECT I5 FROM IntTypes Where Id=2");}

TEST(TableIntTest, ReadNorLSongMin)        { typeGoodTest<signed long>    (-2147483648, "SELECT I6 FROM IntTypes Where Id=2");}
TEST(TableIntTest, ReadNorLSongLongMin)    { typeGoodTest<signed long long>(-2147483648, "SELECT I6 FROM IntTypes Where Id=2");}

TEST(TableIntTest, ReadBigLSongLongMin)    { typeGoodTest<signed long long>(-9223372036854775808ULL, "SELECT I7 FROM IntTypes Where Id=2");}

// Test MAX values
// The signed values
// 127, 32767, 8388607, 2147483647, 2147483647, 9223372036854775807);
TEST(TableIntTest, ReadTinyCharMax)        { typeGoodTest<char>           (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinySCharMax)       { typeGoodTest<signed char>    (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinySShortMax)      { typeGoodTest<signed short>   (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinySIntMax)        { typeGoodTest<signed int>     (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinySLongMax)       { typeGoodTest<signed long>    (127, "SELECT I2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinySLongLongMax)   { typeGoodTest<signed long long>(127, "SELECT I2 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadSmallSShortMax)     { typeGoodTest<signed short>   (32767, "SELECT I3 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadSmallSIntMax)       { typeGoodTest<signed int>     (32767, "SELECT I3 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadSmallSLongMax)      { typeGoodTest<signed long>    (32767, "SELECT I3 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadSmallSLongLongMax)  { typeGoodTest<signed long long>(32767, "SELECT I3 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadMedSIntMax)         { typeGoodTest<signed int>     (8388607, "SELECT I4 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadMedSLongMax)        { typeGoodTest<signed long>    (8388607, "SELECT I4 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadMedSLongLongMax)    { typeGoodTest<signed long long>(8388607, "SELECT I4 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadIntSLongMax)        { typeGoodTest<signed long>    (2147483647, "SELECT I5 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadIntSLongLongMax)    { typeGoodTest<signed long long>(2147483647, "SELECT I5 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadNorSLongMax)        { typeGoodTest<signed long>    (2147483647, "SELECT I6 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadNorSLongLongMax)    { typeGoodTest<signed long long>(2147483647, "SELECT I6 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadBigSLongLongMax)    { typeGoodTest<signed long long>(9223372036854775807ULL, "SELECT I7 FROM IntTypes Where Id=3");}

// Test MAX unsigned values
// The signed values
// 255, 65535, 16777215, 4294967295, 4294967295, 18446744073709551615,
TEST(TableIntTest, ReadTinyUCharMax)       { typeGoodTest<unsigned char>  (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinyUShortMax)      { typeGoodTest<unsigned short> (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinyUIntMax)        { typeGoodTest<unsigned int>   (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinyULongMax)       { typeGoodTest<unsigned long>  (255, "SELECT U2 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadTinyULongLongMax)   { typeGoodTest<unsigned long long>(255, "SELECT U2 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadSmallUShortMax)     { typeGoodTest<unsigned short> (65535, "SELECT U3 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadSmallUIntMax)       { typeGoodTest<unsigned int>   (65535, "SELECT U3 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadSmallULongMax)      { typeGoodTest<unsigned long>  (65535, "SELECT U3 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadSmallULongLongMax)  { typeGoodTest<unsigned long long>(65535, "SELECT U3 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadMedUIntMax)         { typeGoodTest<unsigned int>   (16777215, "SELECT U4 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadMedULongMax)        { typeGoodTest<unsigned long>  (16777215, "SELECT U4 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadMedULongLongMax)    { typeGoodTest<unsigned long long>(16777215, "SELECT U4 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadIntULongMax)        { typeGoodTest<unsigned long>  (4294967295, "SELECT U5 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadIntULongLongMax)    { typeGoodTest<unsigned long long>(4294967295, "SELECT U5 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadNorULongMax)        { typeGoodTest<unsigned long>  (4294967295, "SELECT U6 FROM IntTypes Where Id=3");}
TEST(TableIntTest, ReadNorULongLongMax)    { typeGoodTest<unsigned long long>(4294967295, "SELECT U6 FROM IntTypes Where Id=3");}

TEST(TableIntTest, ReadBigULongLongMax)    { typeGoodTest<unsigned long long>(18446744073709551615ULL, "SELECT U7 FROM IntTypes Where Id=3");}

