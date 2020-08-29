
#include "PackageBuffer.h"
#include "ConectWriter.h"
#include "test/MockStream.h"
#include "ThorsDB/Endian.h"
#include "gtest/gtest.h"

using ThorsAnvil::DB::Postgres::PackageBuffer;
using ThorsAnvil::DB::Postgres::ConectWriter;
using ThorsAnvil::DB::Util::convertToHost;

TEST(ConectWriterTest, OpenMessageGetType)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x04" // size
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    writer.sendMessage('a');

    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}

TEST(ConectWriterTest, WriteSingleMessage)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    std::vector<char> m1 {'1', '2'};
    writer.writeByte(m1);
    writer.sendMessage('a');

    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}

TEST(ConectWriterTest, WriteTwoMessages)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x06" // size
							 "12"
							 "b"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    std::vector<char> m1 {'1', '2'};
    writer.writeByte(m1);
    writer.sendMessage('a');
    writer.writeByte(m1);
    writer.sendMessage('b');
    
    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}

TEST(ConectWriterTest, ReadInteger16)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x12" // size
							 "\x12\x34"
							 "\x56\x78"
                             "\x9A\xBC"
							 "\x5E\xF0"
                             "\x1F\x2E"
                             "\x3D\x4C"
                             "\x5B\x6A"
                             "b"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    writer.writeInt16(4660);

    std::vector<std::int16_t>   m1 {0x5678, static_cast<std::int16_t>(std::uint16_t(0x9ABC)), 0x5EF0, 0x1F2E, 0x3D4C, 0x5B6A};
    writer.writeInt16Vector(m1);
    writer.sendMessage('a');

    std::vector<char> m2 {'1', '2'};
    writer.writeByte(m2);
    writer.sendMessage('b');

    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}
TEST(ConectWriterTest, ReadInteger32)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x10" // size
							 "\x56\x78\x9A\xBC"
							 "\x5E\xF0\x1F\x2E"
                             "\x3D\x4C\x5B\x6A"
                             "b"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    writer.writeInt32(0x56789ABC);

    std::vector<std::int32_t>   m1 {0x5EF01F2E, 0x3D4C5B6A};
    writer.writeInt32Vector(m1);
    writer.sendMessage('a');

    std::vector<char> m2 {'1', '2'};
    writer.writeByte(m2);
    writer.sendMessage('b');

    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}
TEST(ConectWriterTest, ReadInteger64)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x24" // size
							 "\x12\x34\x56\x78\x9A\xBC\x6A\x5B"
							 "\x5E\xF0\x1F\x2E\x3D\x4C\x5B\x6A"
							 "\x12\x34\x56\x78\x9A\xBC\x6A\x5B"
							 "\x5E\xF0\x1F\x2E\x3D\x4C\x5B\x6A"
                             "b"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    writer.writeInt64(0x123456789ABC6A5B);

    std::vector<std::int64_t>   m1 {0x5EF01F2E3D4C5B6A, 0x123456789ABC6A5B, 0x5EF01F2E3D4C5B6A};
    writer.writeInt64Vector(m1);
    writer.sendMessage('a');

    std::vector<char> m2 {'1', '2'};
    writer.writeByte(m2);
    writer.sendMessage('b');

    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}
TEST(ConectWriterTest, ReadIntegerUnaligned)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x15" // size
							 "11\0"									// 3 byte of data
							 "\x12\x34"								// 16 bit big endian 4660
							 "\x56\x78\x9A\xBC"						// 32 bit big endian 1450744508
							 "\x5E\xF0\x1F\x2E\x3D\x4C\x5B\x6A"		// 64 bit big endian 6841002117433154410
                             "b"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    //ASSERT_EQ('a',                  writer.getMessage());
	//ASSERT_EQ("11",                 writer.readString());
	//ASSERT_EQ(4660,                 writer.readInt16());
	//ASSERT_EQ(1450744508,           writer.readInt32());
	//ASSERT_EQ(6841002117433154410,  writer.readInt64()); //773845214
    writer.writeString("11");
    writer.writeInt16(4660);
    writer.writeInt32(1450744508);
    writer.writeInt64(6841002117433154410);
    writer.sendMessage('a');

    //ASSERT_EQ('b', writer.getMessage());
    std::vector<char> m2 {'1', '2'};
    writer.writeByte(m2);
    writer.sendMessage('b');

    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}
TEST(ConectWriterTest, ReadString)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x16" // size
							 "11\0"				
                             "Another String\0"
                             "c"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[sizeof(data)];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);

    //ASSERT_EQ('a',                  writer.getMessage());
	//ASSERT_EQ("11",                 writer.readString());
    //ASSERT_EQ("Another String",     writer.readString());
    writer.writeString("11");
    writer.writeString("Another String");
    writer.sendMessage('a');

    std::vector<char> m2 {'1', '2'};
    writer.writeByte(m2);
    writer.sendMessage('c');

    for(int loop = 0;loop < (sizeof(data) - 1); ++loop)
    {
        ASSERT_EQ(data[loop], static_cast<char>(result[loop]));
    }
}
