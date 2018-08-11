
#include "PackageBuffer.h"
#include "test/MockStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::MySQL::PackageBuffer;
using MySqlBuf=PackageBuffer;

TEST(PackageBufferTest, isEmpty)
{
    char const      data[] = "\x00\x00\x00" // size
                             "\x00"         // id
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    MySqlBuf        mysqlBuffer(buffer);

    ASSERT_TRUE(mysqlBuffer.isEmpty());
}
TEST(PackageBufferTest, read1Block)
{
    char const      data[] = "\x10\x00\x00" // size
                             "\x00"         // id
                             "1234567890ABCDEF" // Note: '\0' is part of data
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result); // The '\0' is not part of the data
    MySqlBuf        mysqlBuffer(buffer);

    ASSERT_FALSE(mysqlBuffer.isEmpty());
    char    dst[16];
    mysqlBuffer.read(dst, 16);
    ASSERT_TRUE(mysqlBuffer.isEmpty());
}
TEST(PackageBufferTest, read2BlockSecondWithZero)
{
    char            block1Header[] = {'\xFF', '\xFF', '\xFF', '\x00'}; // size + id
    char            block2Header[] = {'\x00', '\x00', '\x00', '\x01'}; // size + id
    std::string     data;
    data.reserve(4 + 0xFFFFFFLL + 4);
    data.insert(data.end(), std::begin(block1Header), std::end(block1Header));
    for(unsigned long loop = 0; loop < 0xFFFFFFLL; ++loop) {
        data.insert(data.end(),'X');
    }
    data.insert(data.end(), std::begin(block2Header), std::end(block2Header));

    unsigned char   result[4];
    MockStream      buffer(&data[0], data.size(), result);
    MySqlBuf        mysqlBuffer(buffer);

    ASSERT_FALSE(mysqlBuffer.isEmpty());
    char    dst[0x100];
    // Note: 0x10101 * 0xFF = 0xFFFFFF
    for(unsigned long loop = 0;loop < 0x10101; ++loop) {
        mysqlBuffer.read(dst, 0xFF);
    }
    ASSERT_TRUE(mysqlBuffer.isEmpty());
}
TEST(PackageBufferTest, read2BlockSecondWithTen)
{
    char            block1Header[] = {'\xFF', '\xFF', '\xFF', '\x00'}; // size + id
    char            block2Header[] = {'\x0A', '\x00', '\x00', '\x01'}; // size + id
    std::string     data;
    data.reserve(4 + 0xFFFFFFLL + 4);
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
    MySqlBuf        mysqlBuffer(buffer);

    ASSERT_FALSE(mysqlBuffer.isEmpty());
    char    dst[0x100];
    // Note: 0x10101 * 0xFF = 0xFFFFFF
    for(unsigned long loop = 0;loop < 0x10101; ++loop) {
        mysqlBuffer.read(dst, 0xFF);
    }
    ASSERT_FALSE(mysqlBuffer.isEmpty());
    mysqlBuffer.read(dst, 0x0A);
    ASSERT_TRUE(mysqlBuffer.isEmpty());
    ASSERT_THROW(mysqlBuffer.read(dst, 1), std::domain_error);
}

TEST(PackageBufferTest, writeZero)
{
    char const      data[] = "";
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.flush();
    ASSERT_EQ(4, buffer.writLen());
    ASSERT_EQ(0, result[0]);
    ASSERT_EQ(0, result[1]);
    ASSERT_EQ(0, result[2]);
    ASSERT_EQ(0, result[3]);
}
TEST(PackageBufferTest, writeOneBlock)
{
    char const      data[] = "";
    unsigned char   result[14];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.write("1234567890",10);
    mysqlBuffer.flush();
    ASSERT_EQ(14, buffer.writLen());
    ASSERT_EQ(10, result[0]);
    ASSERT_EQ(0, result[1]);
    ASSERT_EQ(0, result[2]);
    ASSERT_EQ(0, result[3]);
    ASSERT_EQ('1', result[4]);
    ASSERT_EQ('2', result[5]);
    ASSERT_EQ('3', result[6]);
    ASSERT_EQ('4', result[7]);
    ASSERT_EQ('5', result[8]);
    ASSERT_EQ('6', result[9]);
    ASSERT_EQ('7', result[10]);
    ASSERT_EQ('8', result[11]);
    ASSERT_EQ('9', result[12]);
    ASSERT_EQ('0', result[13]);
}
TEST(PackageBufferTest, writeTwoBlockZero)
{
    char const      data[] = "";
    std::vector<unsigned char>   result(4 + 0xFFFFFF + 5, '\0');
    MockStream      buffer(data, sizeof(data), &result[0]);
    MySqlBuf        mysqlBuffer(buffer);
    char            src[256] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,

        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,

        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,

        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    };

    long long w = 0;
    for(unsigned long loop = 0; loop < 0x10101; ++loop) {
        mysqlBuffer.write(src, 0xFF);
        w += 0xFF;
    }
    mysqlBuffer.flush();
    ASSERT_EQ(4 + 0xFFFFFFLL + 4, buffer.writLen());
    ASSERT_EQ(0xFF, result[0]);
    ASSERT_EQ(0xFF, result[1]);
    ASSERT_EQ(0xFF, result[2]);
    ASSERT_EQ(0,    result[3]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 0]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 1]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 2]);
    ASSERT_EQ(0x01, result[0xFFFFFF + 4 + 3]);
}


TEST(PackageBufferTest, writeTwoBlockTenInOneGo)
{
    static char    hugeBlockData[0x2000002] = {0};
    char const      data[] = "";
    std::vector<unsigned char>   result(4 + 0xFFFFFF + 4+ 0xFFFFFF + 10, '\0');
    MockStream      buffer(data, sizeof(data), &result[0]);
    MySqlBuf        mysqlBuffer(buffer);

    long long w = 0;
    mysqlBuffer.write(hugeBlockData, sizeof(hugeBlockData));
    mysqlBuffer.flush();
    ASSERT_EQ(4 + 0xFFFFFFLL + 4 + 0xFFFFFF + 4 + 4, buffer.writLen());
}
TEST(PackageBufferTest, flush)
{
    char const      data[] = "\x00\x00\x00" // size
                             "\x01"         // id
                             ;
    unsigned char   result[8];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.flush();
    ASSERT_EQ(4, buffer.writLen());
    mysqlBuffer.startNewConversation(true);
    mysqlBuffer.flush();    // Flush OK after a startNewConversation()
    ASSERT_EQ(8, buffer.writLen());
    // But should fail if done twice without a reset
    ASSERT_THROW(mysqlBuffer.flush(), std::domain_error);
}
TEST(PackageBufferTest, flushAndWrite)
{
    char const      data[] = "\x00\x00\x00" // size
                             "\x01"         // id
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.flush();
    ASSERT_EQ(4, buffer.writLen());
    ASSERT_THROW(mysqlBuffer.write("10", 2), std::domain_error);
}
TEST(PackageBufferTest, resetWithFlush)
{
    char const      data[] = "\x00\x00\x00" // size
                             "\x01"         // id
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.flush();
    ASSERT_EQ(4, buffer.writLen());
    mysqlBuffer.reset();
}
TEST(PackageBufferTest, readRemainingData)
{
    char const      data[] = "\x0A\x00\x00" // size
                             "\x00"         // id
                             "1234567890"
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    std::string remaining = mysqlBuffer.readRemainingData();
    ASSERT_EQ("1234567890", remaining);
}
TEST(PackageBufferTest, dropDataPreCheck)
{
    char const      data[] = "\x0A\x00\x00" // size
                             "\x00"         // id
                             "\x01" "A"     // String
                             "1234567890"   // Remaining String
                             ;
    MockStream      buffer(data, sizeof(data) - 1);
    MySqlBuf        mysqlBuffer(buffer);

    char            result[4];
    mysqlBuffer.read(result, 2);
    ASSERT_THROW(mysqlBuffer.reset(), std::domain_error);
}
TEST(PackageBufferTest, dropData)
{
    char const      data[] = "\x0A\x00\x00" // size
                             "\x00"         // id
                             "\x01" "A"     // String
                             "1234567890"   // Remaining String
                             ;
    MockStream      buffer(data, sizeof(data) - 1);
    MySqlBuf        mysqlBuffer(buffer);

    char            result[4];
    mysqlBuffer.read(result, 2);

    // Check the test dropDataPreCheck.
    // Because there was data left the reset causes an exception.
    // Now we drop the data before the reset. This means we should
    // be at the end of the package and reset is now OK
    mysqlBuffer.drop();
    ASSERT_NO_THROW(mysqlBuffer.reset());
}
TEST(PackageBufferTest, dropDataNoneRead)
{
    char const      data[] = "\x0A\x00\x00" // size
                             "\x00"         // id
                             "\x01" "A"     // String
                             "1234567890"   // Remaining String
                             ;
    MockStream      buffer(data, sizeof(data) - 1);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.drop();
    ASSERT_NO_THROW(mysqlBuffer.reset());
}
TEST(PackageBufferTest, dropDataFromHugePackageSecondEmpty)
{
    std::vector<char>      data(4 + 0xFFFFFF + 4 + 0);
    data[0]                 = '\xFF';
    data[1]                 = '\xFF';
    data[2]                 = '\xFF';       // Size 0xFFFFFF
    data[3]                 = '\x00';       // id 0
    data[4+0xFFFFFF + 0]    = '\x00';
    data[4+0xFFFFFF + 1]    = '\x00';
    data[4+0xFFFFFF + 2]    = '\x00';       // Size 0x000000
    data[4+0xFFFFFF + 3]    = '\x01';       // id 1

    MockStream      buffer(&data[0], data.size());
    MySqlBuf        mysqlBuffer(buffer);

    char        temp[10];
    mysqlBuffer.read(temp, 10);

    mysqlBuffer.drop();
    ASSERT_NO_THROW(mysqlBuffer.reset());
    ASSERT_EQ(4 + 0xFFFFFF + 4 + 0, buffer.readLen());
}
TEST(PackageBufferTest, dropDataFromHugePackageSecondWithSome)
{
    std::vector<char>      data(4 + 0xFFFFFF + 4 + 10);
    data[0]                 = '\xFF';
    data[1]                 = '\xFF';
    data[2]                 = '\xFF';       // Size 0xFFFFFF
    data[3]                 = '\x00';       // id 0
    data[4+0xFFFFFF + 0]    = '\x0A';
    data[4+0xFFFFFF + 1]    = '\x00';
    data[4+0xFFFFFF + 2]    = '\x00';       // Size 0x000000
    data[4+0xFFFFFF + 3]    = '\x01';       // id 1

    MockStream      buffer(&data[0], data.size());
    MySqlBuf        mysqlBuffer(buffer);

    char        temp[10];
    mysqlBuffer.read(temp, 10);

    mysqlBuffer.drop();
    ASSERT_NO_THROW(mysqlBuffer.reset());
    ASSERT_EQ(4 + 0xFFFFFF + 4 + 10, buffer.readLen());
}

