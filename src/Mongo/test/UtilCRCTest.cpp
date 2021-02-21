#include "UtilCRC32.h"
#include <gtest/gtest.h>

using namespace std::string_literals;
using ThorsAnvil::DB::Mongo::CRC32_Checksum;
using ThorsAnvil::DB::Mongo::CRC32C_Checksum;



TEST(UtilCRCTest, NumberTest)
{
    std::string data = "123456789";

    CRC32_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0xCBF43926ul), crc.checksum());
}

TEST(UtilCRCTest, EmptyString)
{
    std::string data = "";

    CRC32_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0x00000000ul), crc.checksum());
}

TEST(UtilCRCTest, SingleSpace)
{
    std::string data = " ";

    CRC32_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0xE96CCF45ul), crc.checksum());
}

TEST(UtilCRCTest, QuickBrownFox)
{
    std::string data = "The quick brown fox jumps over the lazy dog";

    CRC32_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0x414FA339ul), crc.checksum());
}

TEST(UtilCRCTest, ShortString)
{
    std::string data = "various CRC algorithms input data";

    CRC32_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0x9BD366AEul), crc.checksum());
}

TEST(UtilCRCTest, CRC32C_AllZero)
{
    std::string data = 
        "\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00"s;

    CRC32C_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0x8a9136aa), crc.checksum());
}

TEST(UtilCRCTest, CRC32C_All255)
{
    std::string data = 
        "\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff"s;

    CRC32C_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0x62a8ab43), crc.checksum());
}

TEST(UtilCRCTest, CRC32C_Incrementing)
{
    std::string data = 
        "\x00\x01\x02\x03\x04\x05\x06\x07"
        "\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17"
        "\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"s;

    CRC32C_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0x46dd794e), crc.checksum());
}

TEST(UtilCRCTest, CRC32C_Decrementing)
{
    std::string data = 
        "\x1f\x1e\x1d\x1c\x1b\x1a\x19\x18"
        "\x17\x16\x15\x14\x13\x12\x11\x10"
        "\x0f\x0e\x0d\x0c\x0b\x0a\x09\x08"
        "\x07\x06\x05\x04\x03\x02\x01\x00"s;

    CRC32C_Checksum   crc;
    crc.append(&data[0], data.size());

    EXPECT_EQ(static_cast<uint32_t>(0x113fdb5c), crc.checksum());
}

