#include "UtilCRC32.h"
#include <gtest/gtest.h>


TEST(UtilCRCTest, NumberTest)
{
    std::uint32_t   result = ThorsAnvil::DB::Mongo::crc32CheckSum("123456789", 9);
    EXPECT_EQ(0xCBF43926ul, result);
}

TEST(UtilCRCTest, EmptyString)
{
    std::uint32_t   result = ThorsAnvil::DB::Mongo::crc32CheckSum("", 0);
    EXPECT_EQ(0x00000000ul, result);
}

TEST(UtilCRCTest, SingleSpace)
{
    std::uint32_t   result = ThorsAnvil::DB::Mongo::crc32CheckSum(" ", 1);
    EXPECT_EQ(0xE96CCF45ul, result);
}

TEST(UtilCRCTest, QuickBrownFox)
{
    std::uint32_t   result = ThorsAnvil::DB::Mongo::crc32CheckSum("The quick brown fox jumps over the lazy dog", 43);
    EXPECT_EQ(0x414FA339ul, result);
}

TEST(UtilCRCTest, ShortString)
{
    std::uint32_t   result = ThorsAnvil::DB::Mongo::crc32CheckSum("various CRC algorithms input data", 33);
    EXPECT_EQ(0x9BD366AEul, result);
}

