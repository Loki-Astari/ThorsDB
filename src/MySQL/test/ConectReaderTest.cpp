
#include "ConectReader.h"
#include "ThorsDBCommon/StreamInterface.h"
#include "RespPackageOK.h"
#include "test/MockStream.h"

#include "gtest/gtest.h"
#include "ThorMySQL.h"


using ThorsAnvil::DB::MySQL::ConectReader;

TEST(ConectReaderTest, isEmpty)
{
    char const          data[] = "";
    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);
    char                readBuffer[10];

    ASSERT_EQ(false, reader.isEmpty());
    reader.read(readBuffer, 1);
    ASSERT_EQ(true, reader.isEmpty());
}

TEST(ConectReaderTest, fixedLengthInteger)
{
    char                len1;
    int                 len2;
    int                 len3;
    int                 len4;
    long long           len8;

    char const          data[] = "\x01"                 // Len1
                                 "\x01\x02"             // Len2
                                 "\x01\x02\x03"         // Len3
                                 "\x01\x02\x03\x04"     // Len4
                                 "\x01\x02\x03\x04"     // Len8
                                 "\x05\x06\x07\x08";

    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);

    len1    = reader.fixedLengthInteger<1>();
    ASSERT_EQ(1, len1);
    len2    = reader.fixedLengthInteger<2>();
    ASSERT_EQ(2 * 256 + 1, len2);
    len3    = reader.fixedLengthInteger<3>();
    ASSERT_EQ(3 * 256 * 256 + 2 * 256 + 1, len3);
    len4    = reader.fixedLengthInteger<4>();
    ASSERT_EQ(4 * 256 * 256 *256 + 3 * 256 * 256 + 2 * 256 + 1, len4);
    len8    = reader.fixedLengthInteger<8>();
    ASSERT_EQ(8L * 256 * 256 * 256 * 256 * 256 * 256 * 256
            + 7L * 256 * 256 * 256 * 256 * 256 * 256
            + 6L * 256 * 256 * 256 * 256 * 256
            + 5L * 256 * 256 * 256 * 256
            + 4L * 256 * 256 * 256
            + 3L * 256 * 256
            + 2L * 256
            + 1L,
            len8);
}

TEST(ConectReaderTest, lengthEncodedInteger)
{
    char const          data[] = "\xEF"             // largest 1 byte number
                                 "\xFC\xFD\x00"     // Small two byte number
                                 "\xFC\xFF\xFF"     // Big   two byte number
                                 "\xFD\x01\xFF\xFF" // Small three byte number
                                 "\xFD\xFF\xFF\xFF" // Big   three byte number
                                 "\xFE\x01\xFF\xFF\xFF"
                                     "\x00\x00\x00\x00"     // Small 8 byte number
                                 "\xFE\xFF\xFF\xFF\xFF"
                                     "\xFF\xFF\xFF\xFF";    // Big Eight byte number
                        ;
    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);

    unsigned long value   = reader.lengthEncodedInteger();
    ASSERT_EQ(14 * 16 + 15, value);

    value   = reader.lengthEncodedInteger();
    ASSERT_EQ(253, value);

    value   = reader.lengthEncodedInteger();
    ASSERT_EQ(255 * 256 + 255, value);

    value   = reader.lengthEncodedInteger();
    ASSERT_EQ(255 * 256 * 256 + 255 * 256 + 1, value);

    value   = reader.lengthEncodedInteger();
    ASSERT_EQ(255 * 256 * 256 + 255 * 256 + 255, value);

    value   = reader.lengthEncodedInteger();
    ASSERT_EQ(255L * 256 * 256 * 256
            + 255L * 256 * 256
            + 255L * 256
            + 1L,
            value);

    value   = reader.lengthEncodedInteger();
    ASSERT_EQ(
            + 255UL * 256 * 256 * 256 * 256 * 256 * 256 * 256
            + 255UL * 256 * 256 * 256 * 256 * 256 * 256
            + 255UL * 256 * 256 * 256 * 256 * 256
            + 255UL * 256 * 256 * 256 * 256
            + 255UL * 256 * 256 * 256
            + 255UL * 256 * 256
            + 255UL * 256
            + 255UL,
            value);
}
TEST(ConectReaderTest, lengthEncodedIntegerFail)
{
    char const          data[] = "\xFA\xFB\xFF";
    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);

    ASSERT_THROW(reader.lengthEncodedInteger(), std::domain_error);    // FA
    ASSERT_THROW(reader.lengthEncodedInteger(), std::domain_error);    // FB
    ASSERT_THROW(reader.lengthEncodedInteger(), std::domain_error);    // FC
}

TEST(ConectReaderTest, fixedLengthString)
{
    char const          data[] = "12345";
    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);

    std::string     result = reader.fixedLengthString(4);
    ASSERT_EQ("1234", result);
}

TEST(ConectReaderTest, nulTerminatedString)
{
    char const          data[] = "12345";
    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);

    std::string     result = reader.nulTerminatedString();
    ASSERT_EQ("12345", result);
}

TEST(ConectReaderTest, variableLengthString)
{
    char const          data[] = "12345";
    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);

    std::string     result = reader.variableLengthString(4);
    ASSERT_EQ("1234", result);
}

TEST(ConectReaderTest, lengthEncodedString)
{
    char const          data[] = "\05" "12345";
    MockStream          buffer(data, sizeof(data));
    ConectReader        reader(buffer);

    std::string     result = reader.lengthEncodedString();
    ASSERT_EQ("12345", result);
}

TEST(ConectReaderTest, restOfPacketString)
{
    char const          data[] = "12345";
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    std::string     result = reader.restOfPacketString();
    ASSERT_EQ("12345", result);
}

TEST(ConectReaderTest, lengthEncodedBlob)
{
    char const          data[] = "\05" "12345";
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    std::vector<char>   result = reader.lengthEncodedBlob();
    ASSERT_EQ(5,   result.size());
    ASSERT_EQ('1', result[0]);
    ASSERT_EQ('2', result[1]);
    ASSERT_EQ('3', result[2]);
    ASSERT_EQ('4', result[3]);
    ASSERT_EQ('5', result[4]);
}

TEST(ConectReaderTest, readDateIntoTimeBag11)
{
    char const          data[] = "\013"         // Size
                                 "\xDF\x07"     // Year 2015
                                 "\x04"         // April
                                 "\x19"         // 25
                                 "\x06"         // 6 AM
                                 "\x30"         // 48 min
                                 "\x2A"         // 42 Secs
                                 "\x04\x00\x00\x00" // 4 micro secs
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    ThorsAnvil::DB::MySQL::MySQLTimeBag tb = reader.readDateIntoTimeBag();

    ASSERT_EQ(2015, tb.year);
    ASSERT_EQ(4,    tb.month);
    ASSERT_EQ(25,   tb.day);
    ASSERT_EQ(6,    tb.hour);
    ASSERT_EQ(48,   tb.minute);
    ASSERT_EQ(42,   tb.second);
    ASSERT_EQ(4,    tb.uSecond);
}
TEST(ConectReaderTest, readDateIntoTimeBag7)
{
    char const          data[] = "\007"         // Size
                                 "\xDF\x07"     // Year 2015
                                 "\x04"         // April
                                 "\x19"         // 25
                                 "\x06"         // 6 AM
                                 "\x30"         // 48 min
                                 "\x2A"         // 42 Secs
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    ThorsAnvil::DB::MySQL::MySQLTimeBag tb = reader.readDateIntoTimeBag();

    ASSERT_EQ(2015, tb.year);
    ASSERT_EQ(4,    tb.month);
    ASSERT_EQ(25,   tb.day);
    ASSERT_EQ(6,    tb.hour);
    ASSERT_EQ(48,   tb.minute);
    ASSERT_EQ(42,   tb.second);
    ASSERT_EQ(0,    tb.uSecond);
}
TEST(ConectReaderTest, readDateIntoTimeBag4)
{
    char const          data[] = "\004"         // Size
                                 "\xDF\x07"     // Year 2015
                                 "\x04"         // April
                                 "\x19"         // 25
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    ThorsAnvil::DB::MySQL::MySQLTimeBag tb = reader.readDateIntoTimeBag();

    ASSERT_EQ(2015, tb.year);
    ASSERT_EQ(4,    tb.month);
    ASSERT_EQ(25,   tb.day);
    ASSERT_EQ(0,    tb.hour);
    ASSERT_EQ(0,    tb.minute);
    ASSERT_EQ(0,    tb.second);
    ASSERT_EQ(0,    tb.uSecond);
}
TEST(ConectReaderTest, readDateIntoTimeBagFail)
{
    char const          data[] = "\020"         // Size
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    ASSERT_THROW(reader.readDateIntoTimeBag(), std::domain_error);
}

TEST(ConectReaderTest, readDate)
{
    char const          data[] = "\013"         // Size
                                 "\xDF\x07"     // Year 2015
                                 "\x04"         // April
                                 "\x19"         // 25
                                 "\x06"         // 6 AM
                                 "\x30"         // 48 min
                                 "\x2A"         // 42 Secs
                                 "\x04\x00\x00\x00" // 4 micro secs
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    std::time_t time = reader.readDate();

    ASSERT_EQ(1429944522, time);
}

TEST(ConectReaderTest, readRel)
{
    char const          data[] = "\014"             // Size
                                 "\x00"             // Negative test
                                 "\xA6\x40\x00\x00" // Days
                                 "\x06"             // Hours
                                 "\x30"             // Minutes
                                 "\x2A"             // Seconds
                                 "\x04\x00\x00\x00" // uSeconds
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    unsigned long value = reader.readRel();
    ASSERT_EQ(1429944522, value);
}

TEST(ConectReaderTest, readRelMicro)
{
    char const          data[] = "\014"             // Size
                                 "\x00"             // Negative test
                                 "\xA6\x40\x00\x00" // Days
                                 "\x06"             // Hours
                                 "\x30"             // Minutes
                                 "\x2A"             // Seconds
                                 "\x04\x00\x00\x00" // uSeconds
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    unsigned long long value = reader.readRelMicro();
    ASSERT_EQ(1429944522004, value);
}

TEST(ConectReaderTest, readTimeIntoTimeBag12)
{
    char const          data[] = "\014"             // Size
                                 "\x00"             // Negative test
                                 "\xA6\x40\x00\x00" // Days
                                 "\x06"             // Hours
                                 "\x30"             // Minutes
                                 "\x2A"             // Seconds
                                 "\x04\x00\x00\x00" // uSeconds
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    unsigned long value = reader.readRel();
    ASSERT_EQ(1429944522,    value);
}
TEST(ConectReaderTest, readTimeIntoTimeBag8)
{
    char const          data[] = "\010"             // Size
                                 "\x00"             // Negative test
                                 "\xA6\x40\x00\x00" // Days
                                 "\x06"             // Hours
                                 "\x30"             // Minutes
                                 "\x2A"             // Seconds
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    unsigned long long value = reader.readRelMicro();
    ASSERT_EQ(1429944522000,    value);
}
TEST(ConectReaderTest, readTimeIntoTimeBagFail)
{
    char const          data[] = "\011"             // Size
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    ASSERT_THROW(reader.readRelMicro(), std::domain_error);
}
TEST(ConectReaderTest, readTimeIntoTimeBagNegative)
{
    char const          data[] = "\010"             // Size
                                 "\x01"             // Negative test
                                 "\xA6\x40\x00\x00" // Days
                                 "\x06"             // Hours
                                 "\x30"             // Minutes
                                 "\x2A"             // Seconds
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    unsigned long long value = reader.readRelMicro();
    ASSERT_EQ(1429944522000,    value);
}
TEST(ConectReaderTest, readTimeIntoTimeBagNegativeFail)
{
    char const          data[] = "\010"             // Size
                                 "\x02"             // Negative test
                                 "\xA6\x40\x00\x00" // Days
                                 "\x06"             // Hours
                                 "\x30"             // Minutes
                                 "\x2A"             // Seconds
                                 ;
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);

    ASSERT_THROW(reader.readRelMicro(), std::domain_error);
}

TEST(ConectReaderTest, ThrowOnErrorMessage)
{
    char const          data[] = "\xFF"
                                 "\x01\x00"
                                 "A"
                                 "MNOPQ"
                                 "Error: Better Be This";
    MockStream          buffer(data, sizeof(data) - 1);
    ConectReader        reader(buffer);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    try
    {
        std::unique_ptr<ThorsAnvil::DB::MySQL::RespPackage> message = reader.recvMessage();
        ThorsAnvil::DB::MySQL::downcastUniquePtr<ThorsAnvil::DB::MySQL::RespPackageOK>(std::move(message));
        FAIL();
    }
    catch(std::runtime_error const& e) {
        ASSERT_NE(std::string::npos, std::string(e.what()).find("Better Be This"));
    }
}

TEST(ConectReaderTest, dropData)
{
    char const      data[] = "\x01" "A"     // String
                             "1234567890"   // Remaining String
                             ;
    MockStream      buffer(data, sizeof(data) - 1);
    ConectReader    reader(buffer);

    reader.lengthEncodedString();
    ASSERT_EQ(2, buffer.readLen());

    reader.drop();
    // Just make sure the underlying stream gets moved to the end
    // by calling drop in it.
    ASSERT_EQ(sizeof(data) - 1, buffer.readLen());
}

