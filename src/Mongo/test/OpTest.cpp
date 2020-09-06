
#include <gtest/gtest.h>
#include "Op.h"

#include <iostream>
#include <iomanip>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

template<typename Input>
void testStreamOutput(Input data, std::string const& expected)
{
    std::stringstream   stream;
    stream << make_LE(data);

    std::string output = stream.str();
    EXPECT_EQ(expected, output);
}
template<typename Input>
void testStreamInput(Input expected, std::string const& input)
{
    std::stringstream   stream(input);
    Input   data;
    stream >> make_LE(data);

    EXPECT_EQ(expected, data);
}

TEST(OpTest, StreamOutCharAsLittelIndian)
{
    char data='a';
    testStreamOutput(data, "a\x00\x00\x00"s);
}
TEST(OpTest, StreamOutShortAsLittelIndian)
{
    short data=0xB723;
    testStreamOutput(data, "\x23\xB7\x00\x00"s);
}
TEST(OpTest, StreamOutIntAsLittelIndian)
{
    int data=0xA1A523;
    testStreamOutput(data, "\x23\xA5\xA1\x00"s);
}
TEST(OpTest, StreamOutUCharAsLittelIndian)
{
    unsigned char data='a';
    testStreamOutput(data, "a\x00\x00\x00"s);
}
TEST(OpTest, StreamOutUShortAsLittelIndian)
{
    unsigned short data=0xA501;
    testStreamOutput(data, "\x01\xA5\x00\x00"s);
}
TEST(OpTest, StreamOutUIntAsLittelIndian)
{
    unsigned int data=0xA1A3F3;
    testStreamOutput(data, "\xF3\xA3\xA1\x00"s);
}
///
TEST(OpTest, StreamInCharAsLittelIndian)
{
    char data='a';
    testStreamInput(data, "a\x00\x00\x00"s);
}
TEST(OpTest, StreamInShortAsLittelIndian)
{
    short data=0xB723;
    testStreamInput(data, "\x23\xB7\x00\x00"s);
}
TEST(OpTest, StreamInIntAsLittelIndian)
{
    int data=0xA1A523;
    testStreamInput(data, "\x23\xA5\xA1\x00"s);
}
TEST(OpTest, StreamInUCharAsLittelIndian)
{
    unsigned char data='a';
    testStreamInput(data, "a\x00\x00\x00"s);
}
TEST(OpTest, StreamInUShortAsLittelIndian)
{
    unsigned short data=0xA501;
    testStreamInput(data, "\x01\xA5\x00\x00"s);
}
TEST(OpTest, StreamInUIntAsLittelIndian)
{
    unsigned int data=0xA1A3F3;
    testStreamInput(data, "\xF3\xA3\xA1\x00"s);
}

std::ostream& operator<<(std::ostream& str, HumanReadable<std::string> const& value)
{
    return str << "HR: " << value.object << ":";
}
TEST(OpTest, HumanReadableStreamer)
{
    std::stringstream output;
    std::string       test = "Hi There";
    output << make_hr(test);

    EXPECT_EQ("HR: Hi There:", output.str());
}

