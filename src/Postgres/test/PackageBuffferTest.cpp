
#include "PackageBuffer.h"
#include "test/MockStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::Postgres::PackageBuffer;

TEST(PackageBufferTest, OpenMessageGetType)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x04" // size
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);

    ASSERT_EQ('a', package.getMessage());
}
TEST(PackageBufferTest, OpenMessageGetTypeFailsWhenPackageNotFinished)
{
    char const      data[] = "a1"
                             "\x00\x00\x00\x05" // size
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);

    ASSERT_EQ('a', package.getMessage());
    ASSERT_THROW(
        package.getMessage(),
        std::domain_error
    );
}
TEST(PackageBufferTest, isEmpty)
{
    char const      data[] = "a"
                             "\x00\x00\x00\x04" // size
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);

    ASSERT_EQ('a', package.getMessage());
    ASSERT_TRUE(package.isEmpty());
}
TEST(PackageBufferTest, isEmptyBeforeOpen)
{
    char const      data[] = "b"
                             "\x00\x00\x00\x14" // size
                             "1234567890ABCDEF" // Note: '\0' is part of data
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result); // The '\0' is not part of the data
    PackageBuffer   package(buffer);

    ASSERT_TRUE(package.isEmpty());
}
TEST(PackageBufferTest, readWholeMessage)
{
    char const      data[] = "b"
                             "\x00\x00\x00\x14" // size
                             "1234567890ABCDEF" // Note: '\0' is part of data
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result); // The '\0' is not part of the data
    PackageBuffer   package(buffer);

    ASSERT_EQ('b', package.getMessage());
    char    dst[16];
    package.read(dst, 16);
    ASSERT_TRUE(package.isEmpty());
}
TEST(PackageBufferTest, read2ContigiousBlocks)
{
    char            block1Header[] = {'a', '\x01', '\x00', '\x00', '\x03'}; // id + size
    char            block2Header[] = {'b', '\x00', '\x00', '\x00', '\x0E'}; // id + size
    std::string     data;
    data.reserve(5 + 0xFFFFFFLL + 5);
    data.insert(data.end(), std::begin(block1Header), std::end(block1Header));
    for(unsigned long loop = 0; loop < 0xFFFFFFLL; ++loop) {
        data.insert(data.end(),'X');
    }
    data.insert(data.end(), std::begin(block2Header), std::end(block2Header));
    for(unsigned long loop = 0;loop < 0xA; ++loop) {
        data.insert(data.end(),'Y');
    }

    unsigned char   result[4];
    MockStream      buffer(&data[0], data.size(), result);
    PackageBuffer   package(buffer);

    ASSERT_EQ('a', package.getMessage());
    ASSERT_FALSE(package.isEmpty());
    char    dst[0x100];
    // Note: 0x10101 * 0xFF = 0xFFFFFF
    for(unsigned long loop = 0;loop < 0x10101; ++loop) {
        package.read(dst, 0xFF);
    }
    ASSERT_TRUE(package.isEmpty());

    ASSERT_EQ('b', package.getMessage());
    package.read(dst, 0x0A);
    ASSERT_TRUE(package.isEmpty());
}
TEST(PackageBufferTest, CheckReadPastEndThrows)
{
    char const      data[] = "b"
                             "\x00\x00\x00\x14" // size
                             "1234567890ABCDEF" // Note: '\0' is part of data
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result); // The '\0' is not part of the data
    PackageBuffer   package(buffer);

    ASSERT_EQ('b', package.getMessage());
    char    dst[16];
    package.read(dst, 16);
    ASSERT_TRUE(package.isEmpty());
    ASSERT_THROW(package.read(dst, 1), std::domain_error);
}
TEST(PackageBufferTest, writeEmptyBlock)
{
    char const      data[] = "";
    unsigned char   result[] = "HHHHH";
    MockStream      buffer(data, sizeof(data) - 1, result);
    PackageBuffer   package(buffer);

    package.sendMessage('z');
    //ASSERT_EQ(5, package.writLen());
    ASSERT_EQ('z', result[0]);
    ASSERT_EQ(0, result[1]);
    ASSERT_EQ(0, result[2]);
    ASSERT_EQ(0, result[3]);
    ASSERT_EQ(4, result[4]);
}
TEST(PackageBufferTest, writeOneBlock10Characters)
{
    char const      data[] = "";
    unsigned char   result[] = "1234567890ABCDEF";
    MockStream      buffer(data, sizeof(data) - 1, result);
    PackageBuffer   package(buffer);

    package.write("1234567890",10);
    package.sendMessage('q');
    //ASSERT_EQ(14, package.writLen());
    ASSERT_EQ('q', result[0]);	// Message Type
    ASSERT_EQ(0, result[1]);	// 4 byte Len Network Byte order
    ASSERT_EQ(0, result[2]);
    ASSERT_EQ(0, result[3]);
    ASSERT_EQ(14, result[4]);
    ASSERT_EQ('1', result[5]);	// 10 byte data block
    ASSERT_EQ('2', result[6]);
    ASSERT_EQ('3', result[7]);
    ASSERT_EQ('4', result[8]);
    ASSERT_EQ('5', result[9]);
    ASSERT_EQ('6', result[10]);
    ASSERT_EQ('7', result[11]);
    ASSERT_EQ('8', result[12]);
    ASSERT_EQ('9', result[13]);
    ASSERT_EQ('0', result[14]);
}

TEST(PackageBufferTest, dropData)
{
    char const      data[] = "R"
                             "\x00\x00\x00\x10" // size
                             "A\0"              // String
                             "1234567890"       // Remaining String
                             "T"
                             "\x00\x00\x00\x04";// size
    MockStream      buffer(data, sizeof(data) - 1);
    PackageBuffer   package(buffer);

    // Check the test dropDataPreCheck.
    // Because there was data left the reset causes an exception.
    // Now we drop the data before the reset. This means we should
    // be at the end of the package and reset is now OK
    ASSERT_EQ('R', package.getMessage());
    package.drop();
    package.reset();
    // Next package should be empty
    ASSERT_TRUE(package.isEmpty());
    ASSERT_EQ('T', package.getMessage());
}
TEST(PackageBufferTest, dropDataAfterRead)
{
    char const      data[] = "R"
                             "\x00\x00\x00\x10" // size
                             "A\0"              // String
                             "1234567890"       // Remaining String
                             "T"
                             "\x00\x00\x00\x04";// size
    MockStream      buffer(data, sizeof(data) - 1);
    PackageBuffer   package(buffer);

    char            result[4];

    ASSERT_EQ('R', package.getMessage());
    package.read(result, 2);

    // Check the test dropDataPreCheck.
    // Because there was data left the reset causes an exception.
    // Now we drop the data before the reset. This means we should
    // be at the end of the package and reset is now OK
    package.drop();
    package.reset();
    // Next package should be empty
    ASSERT_TRUE(package.isEmpty());
    ASSERT_EQ('T', package.getMessage());
}
TEST(PackageBufferTest, readRemainingData)
{
    char const      data[] = "Y"
                             "\x00\x00\x00\x0E" // size
                             "1234567890"
                             "P"
                             "\x00\x00\x00\x04";// size
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result);
    PackageBuffer   package(buffer);

    ASSERT_EQ('Y', package.getMessage());
    std::string remaining = package.readRemainingData();
    ASSERT_EQ("1234567890", remaining);
    ASSERT_TRUE(package.isEmpty());

    package.reset();
    ASSERT_EQ('P', package.getMessage());
}

