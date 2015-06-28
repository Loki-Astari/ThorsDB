
#include "PackageBuffer.h"
#include "test/MockStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer;
using MySqlBuf=PackageBufferMySQLDebugBuffer<MockStream>;

TEST(PackageBufferMySQLDebugBufferTest, isEmpty)
{
    char const      data[] = "\x00\x00\x00" // size
                             "\x00"         // id
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data), result);
    MySqlBuf        mysqlBuffer(buffer);

    ASSERT_TRUE(mysqlBuffer.isEmpty());
}
TEST(PackageBufferMySQLDebugBufferTest, read1Block)
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
TEST(PackageBufferMySQLDebugBufferTest, read2BlockSecondWithZero)
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
TEST(PackageBufferMySQLDebugBufferTest, read2BlockSecondWithTen)
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
    ASSERT_THROW(mysqlBuffer.read(dst, 1), std::runtime_error);
}

TEST(PackageBufferMySQLDebugBufferTest, writeZero)
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
TEST(PackageBufferMySQLDebugBufferTest, writeOneBlock)
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
TEST(PackageBufferMySQLDebugBufferTest, writeTwoBlockZero)
{
    char const      data[] = "";
    unsigned char   result[4 + 0xFFFFFF + 4];
    MockStream      buffer(data, sizeof(data), result);
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
    ASSERT_EQ(0xFFFFFFLL + 4 + 4, buffer.writLen());
    ASSERT_EQ(0xFF, result[0]);
    ASSERT_EQ(0xFF, result[1]);
    ASSERT_EQ(0xFF, result[2]);
    ASSERT_EQ(0,    result[3]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 0]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 1]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 2]);
    ASSERT_EQ(0x01, result[0xFFFFFF + 4 + 3]);
}
TEST(PackageBufferMySQLDebugBufferTest, writeTwoBlockTen)
{
    char const      data[] = "";
    unsigned char   result[4 + 0xFFFFFF + 4 + 10];
    MockStream      buffer(data, sizeof(data), result);
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
    mysqlBuffer.write(src, 10);
    mysqlBuffer.flush();
    ASSERT_EQ(0xFFFFFFLL + 4 + 4 + 10, buffer.writLen());
    ASSERT_EQ(0xFF, result[0]);
    ASSERT_EQ(0xFF, result[1]);
    ASSERT_EQ(0xFF, result[2]);
    ASSERT_EQ(0,    result[3]);
    ASSERT_EQ(0x0A, result[0xFFFFFF + 4 + 0]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 1]);
    ASSERT_EQ(0x00, result[0xFFFFFF + 4 + 2]);
    ASSERT_EQ(0x01, result[0xFFFFFF + 4 + 3]);
}
TEST(PackageBufferMySQLDebugBufferTest, flush)
{
    char const      data[] = "\x00\x00\x00" // size
                             "\x01"         // id
                             ;
    unsigned char   result[8];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.flush();
    ASSERT_EQ(4, buffer.writLen());
    mysqlBuffer.reset();
    mysqlBuffer.flush();    // Flush OK after a reset
    ASSERT_EQ(8, buffer.writLen());
    // But should fail if done twice without a reset
    ASSERT_THROW(mysqlBuffer.flush(), std::runtime_error);
}
TEST(PackageBufferMySQLDebugBufferTest, flushAndWrite)
{
    char const      data[] = "\x00\x00\x00" // size
                             "\x01"         // id
                             ;
    unsigned char   result[4];
    MockStream      buffer(data, sizeof(data) - 1, result);
    MySqlBuf        mysqlBuffer(buffer);

    mysqlBuffer.flush();
    ASSERT_EQ(4, buffer.writLen());
    ASSERT_THROW(mysqlBuffer.write("10", 2), std::runtime_error);
}
TEST(PackageBufferMySQLDebugBufferTest, resetWithFlush)
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
TEST(PackageBufferMySQLDebugBufferTest, readRemainingData)
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

