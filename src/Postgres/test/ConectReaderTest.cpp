
#include "PackageBuffer.h"
#include "ConectReader.h"
#include "test/MockStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::DB::Postgres::PackageBuffer;
using ThorsAnvil::DB::Postgres::ConectReader;

TEST(ConectReaderTest, OpenMessageGetType)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x04" // size
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);
    ASSERT_EQ('a', reader.getMessage());
}

TEST(ConectReaderTest, IsEmpty)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x04" // size
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);
    ASSERT_EQ('a', reader.getMessage());
    ASSERT_TRUE(reader.isEmpty());
}

TEST(ConectReaderTest, DropMessageEmptiesIt)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);
    ASSERT_EQ('a', reader.getMessage());
	reader.drop();
    ASSERT_TRUE(reader.isEmpty());
}
TEST(ConectReaderTest, CantReadNextmessageUntilCurrentIsFinished)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x06" // size
							 "12"
							 "b"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);
    ASSERT_EQ('a', reader.getMessage());
	reader.reset();
    ASSERT_EQ('b', reader.getMessage());
	ASSERT_THROW(
		reader.getMessage(),
		std::domain_error
	);
}

TEST(ConectReaderTest, ReadInteger16)
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
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);

    ASSERT_EQ('a',                  reader.getMessage());
	ASSERT_EQ(4660,                 reader.readInt16());
    std::vector<std::int16_t>   input = reader.readInt16Vector(6);
    ASSERT_EQ(6, input.size());
    ASSERT_EQ(0x5678, input[0]);
    ASSERT_EQ(static_cast<std::int16_t>(std::uint16_t(0x9ABC)), input[1]);
    ASSERT_EQ(0x5EF0, input[2]);
    ASSERT_EQ(0x1F2E, input[3]);
    ASSERT_EQ(0x3D4C, input[4]);
    ASSERT_EQ(0x5B6A, input[5]);
    ASSERT_EQ('b', reader.getMessage());
}
TEST(ConectReaderTest, ReadInteger32)
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
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);

    ASSERT_EQ('a',                  reader.getMessage());
	ASSERT_EQ(0x56789ABC,           reader.readInt32());
    std::vector<std::int32_t>   input = reader.readInt32Vector(2);
    ASSERT_EQ(2, input.size());
    ASSERT_EQ(0x5EF01F2E, input[0]);
    ASSERT_EQ(0x3D4C5B6A, input[1]);
    ASSERT_EQ('b', reader.getMessage());
}
TEST(ConectReaderTest, ReadInteger64)
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
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);

    ASSERT_EQ('a',                  reader.getMessage());
	ASSERT_EQ(0x123456789ABC6A5B,  reader.readInt64());
    std::vector<std::int64_t>   input = reader.readInt64Vector(3);
    ASSERT_EQ(3, input.size());
    ASSERT_EQ(0x5EF01F2E3D4C5B6A, input[0]);
    ASSERT_EQ(0x123456789ABC6A5B, input[1]);
    ASSERT_EQ(0x5EF01F2E3D4C5B6A, input[2]);
    ASSERT_EQ('b', reader.getMessage());
}
TEST(ConectReaderTest, ReadIntegerUnaligned)
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
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);

    ASSERT_EQ('a',                  reader.getMessage());
	ASSERT_EQ("11",                 reader.readString());
	ASSERT_EQ(4660,                 reader.readInt16());
	ASSERT_EQ(1450744508,           reader.readInt32());
	ASSERT_EQ(6841002117433154410,  reader.readInt64()); //773845214

    ASSERT_EQ('b', reader.getMessage());
}
TEST(ConectReaderTest, ReadString)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x16" // size
							 "11\0"				
                             "Another String\0"
                             "c"
                             "\x00\x00\x00\x06" // size
							 "12"
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectReader    reader(package);

    ASSERT_EQ('a',                  reader.getMessage());
	ASSERT_EQ("11",                 reader.readString());
    ASSERT_EQ("Another String",     reader.readString());
}

