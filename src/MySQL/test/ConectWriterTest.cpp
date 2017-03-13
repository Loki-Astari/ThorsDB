
#include "ConectWriter.h"
#include "PackageStream.h"

#include "gtest/gtest.h"
#include "test/MockStream.h"


using ThorsAnvil::MySQL::ConectWriter;

TEST(ConectWriter, writeFixedLengthInteger)
{
    char const      data[] = "";
    unsigned char   result[15];
    MockStream      buffer(data, sizeof(data), result);
    ConectWriter    writer(buffer);

    writer.writeFixedLengthInteger<1>(15);
    ASSERT_EQ(result[0], 15);

    writer.writeFixedLengthInteger<2>(500);
    ASSERT_EQ(result[1], 0xF4);
    ASSERT_EQ(result[2], 0x01);

    writer.writeFixedLengthInteger<3>(16777065);
    ASSERT_EQ(result[3], 0x69);
    ASSERT_EQ(result[4], 0xFF);
    ASSERT_EQ(result[5], 0xFF);

    writer.writeFixedLengthInteger<8>(72056947999003736ULL);
    ASSERT_EQ(result[6],    0x58);
    ASSERT_EQ(result[7],    0x54);
    ASSERT_EQ(result[8],    0x14);
    ASSERT_EQ(result[9],    0x95);
    ASSERT_EQ(result[10],   0x69);
    ASSERT_EQ(result[11],   0xFF);
    ASSERT_EQ(result[12],   0xFF);
    ASSERT_EQ(result[13],   0x00);
}

TEST(ConectWriter, writeLengthEncodedInteger)
{
    char const      data[] = "";
    unsigned char   result[17];
    MockStream      buffer(data, sizeof(data), result);
    ConectWriter    writer(buffer);

    writer.writeLengthEncodedInteger(15);   // 1 byte
    ASSERT_EQ(1, buffer.writLen());
    ASSERT_EQ(result[0], 15);

    writer.writeLengthEncodedInteger(258);  // 3 byte
    ASSERT_EQ(4, buffer.writLen());
    ASSERT_EQ(result[1], 0xFC);
    ASSERT_EQ(result[2], 0x02);
    ASSERT_EQ(result[3], 0x01);

    writer.writeLengthEncodedInteger(0x5AB4C2);
    ASSERT_EQ(8, buffer.writLen());     // 4 byte
    ASSERT_EQ(result[4], 0xFD);
    ASSERT_EQ(result[5], 0xC2);
    ASSERT_EQ(result[6], 0xB4);
    ASSERT_EQ(result[7], 0x5A);

    writer.writeLengthEncodedInteger(0x345678ABCD); // 9 byte
    ASSERT_EQ(17, buffer.writLen());
    ASSERT_EQ(result[8],  0xFE);
    ASSERT_EQ(result[9],  0xCD);
    ASSERT_EQ(result[10], 0xAB);
    ASSERT_EQ(result[11], 0x78);
    ASSERT_EQ(result[12], 0x56);
    ASSERT_EQ(result[13], 0x34);
    ASSERT_EQ(result[14], 0x00);
    ASSERT_EQ(result[15], 0x00);
    ASSERT_EQ(result[16], 0x00);
}
TEST(ConectWriter, writeFixedLengthString)
{
    char const      data[] = "";
    unsigned char   result[15];
    MockStream      buffer(data, sizeof(data), result);
    ConectWriter    writer(buffer);

    writer.writeFixedLengthString("BadPl", 5);
    ASSERT_EQ(5, buffer.writLen());
    ASSERT_EQ(result[0], 'B');
    ASSERT_EQ(result[1], 'a');
    ASSERT_EQ(result[2], 'd');
    ASSERT_EQ(result[3], 'P');
    ASSERT_EQ(result[4], 'l');
}
TEST(ConectWriter, writeNullTerminatedString)
{
    char const      data[] = "";
    unsigned char   result[15];
    MockStream      buffer(data, sizeof(data), result);
    ConectWriter    writer(buffer);

    writer.writeNullTerminatedString("Help");
    ASSERT_EQ(5, buffer.writLen());
    ASSERT_EQ(result[0], 'H');
    ASSERT_EQ(result[1], 'e');
    ASSERT_EQ(result[2], 'l');
    ASSERT_EQ(result[3], 'p');
    ASSERT_EQ(result[4], '\0');
}
TEST(ConectWriter, writeVariableLengthString)
{
    char const      data[] = "";
    unsigned char   result[15];
    MockStream      buffer(data, sizeof(data), result);
    ConectWriter    writer(buffer);

    writer.writeVariableLengthString("Plop");
    ASSERT_EQ(4, buffer.writLen());
    ASSERT_EQ(result[0], 'P');
    ASSERT_EQ(result[1], 'l');
    ASSERT_EQ(result[2], 'o');
    ASSERT_EQ(result[3], 'p');
}
TEST(ConectWriter, writeLengthEncodedString)
{
    char const      data[] = "";
    unsigned char   result[15];
    MockStream      buffer(data, sizeof(data), result);
    ConectWriter    writer(buffer);

    writer.writeLengthEncodedString("StepItUp");
    ASSERT_EQ(9, buffer.writLen());
    ASSERT_EQ(result[0], 0x08);
    ASSERT_EQ(result[1], 'S');
    ASSERT_EQ(result[2], 't');
    ASSERT_EQ(result[3], 'e');
    ASSERT_EQ(result[4], 'p');
    ASSERT_EQ(result[5], 'I');
    ASSERT_EQ(result[6], 't');
    ASSERT_EQ(result[7], 'U');
    ASSERT_EQ(result[8], 'p');
}
TEST(ConectWriter, writeHugeStringOneUnder)
{
    char const      data[] = "";
    std::vector<unsigned char> result(0xFFFFFF + 20, '\0');
    MockStream      buffer(data, sizeof(data), &result[0]);
    ConectWriter    writer(buffer);
    std::string     str(0xFFFFFE,'X');

    writer.writeLengthEncodedString(str);
    // 2 bytes Package buffer
    ASSERT_EQ(4 + 0XFFFFFE, buffer.writLen());
}
TEST(ConectWriter, writeHugeStringJustBig)
{
    char const          data[] = "";
    std::vector<unsigned char>   result(0xFFFFFF + 20, '\0');
    MockStream      buffer(data, sizeof(data), &result[0]);
    ConectWriter    writer(buffer);
    std::string     str(0xFFFFFF,'X');

    writer.writeLengthEncodedString(str);
    ASSERT_EQ(4 + 0XFFFFFF, buffer.writLen());
}
TEST(ConectWriter, writeHugeStringOneOver)
{
    char const          data[] = "";
    std::vector<unsigned char>   result(0xFFFFFF + 20, '\0');
    MockStream      buffer(data, sizeof(data), &result[0]);
    ConectWriter    writer(buffer);
    std::string     str(0x1000000,'X');

    writer.writeLengthEncodedString(str);
    ASSERT_EQ(9 + 0X1000000, buffer.writLen());
}

