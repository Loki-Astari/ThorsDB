
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"
#include "RespPackageColumnDefinition.h"
#include "test/MockStream.h"
#include "ConectReader.h"

using ThorsAnvil::DB::MySQL::ConectReader;
using ThorsAnvil::DB::MySQL::RespPackageColumnDefinition;

TEST(RespPackageColumnDefinitionTest, Client41)
{
    char                buffer[] =  "\xFC\x01\x00" "A"  // catalog
                                    "\xFC\x01\x00" "B"  // schema
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "D"  // orgTable
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x01\x00" "F"  // orgName
                                    "\xFC\x0C\x00"      // lengthOfFixedField
                                    "\x01\x02"          // charSet
                                    "\x03\x04\x05\x06"  // columnLength
                                    "\x07"              // type
                                    "\x08\x09"          // flags
                                    "\x0A"              // decimal
                                    "\x00\x00"          // filler
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageColumnDefinition      col(reader);
    EXPECT_TRUE(reader.isEmpty());

    EXPECT_EQ("A", col.catalog);
    EXPECT_EQ("B", col.schema);
    EXPECT_EQ("C", col.table);
    EXPECT_EQ("D", col.orgTable);
    EXPECT_EQ("E", col.name);
    EXPECT_EQ("F", col.orgName);
    EXPECT_EQ(0x0C, col.lengthOfFixedField);
    EXPECT_EQ(0x0201, col.charSet);
    EXPECT_EQ(0x06050403, col.columnLength);
    EXPECT_EQ(0x07, col.type);
    EXPECT_EQ(0x0908, col.flags);
    EXPECT_EQ(0x0A, col.decimal);
    EXPECT_EQ(0x00, col.filler);
    EXPECT_EQ(0, col.defaultValues.size());

    std::stringstream message;
    message << col;
    EXPECT_NE(message.str(), "");
}

TEST(RespPackageColumnDefinitionTest, Client41BadLenFixedField)
{
    char                buffer[] =  "\xFC\x01\x00" "A"  // catalog
                                    "\xFC\x01\x00" "B"  // schema
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "D"  // orgTable
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x01\x00" "F"  // orgName
                                    "\xFC\x0E\xF0"      // lengthOfFixedField
                                    "\x01\x02"          // charSet
                                    "\x03\x04\x05\x06"  // columnLength
                                    "\x07"              // type
                                    "\x08\x09"          // flags
                                    "\x0A"              // decimal
                                    "\x00\x00"          // filler
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    EXPECT_THROW(
        RespPackageColumnDefinition      col(reader),
        ThorsAnvil::Logging::CriticalException
    );
}

TEST(RespPackageColumnDefinitionTest, Client41BadFiller)
{
    char                buffer[] =  "\xFC\x01\x00" "A"  // catalog
                                    "\xFC\x01\x00" "B"  // schema
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "D"  // orgTable
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x01\x00" "F"  // orgName
                                    "\xFC\x0C\x00"      // lengthOfFixedField
                                    "\x01\x02"          // charSet
                                    "\x03\x04\x05\x06"  // columnLength
                                    "\x07"              // type
                                    "\x08\x09"          // flags
                                    "\x0A"              // decimal
                                    "\x00\x40"          // filler
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    EXPECT_THROW(
        RespPackageColumnDefinition      col(reader),
        ThorsAnvil::Logging::CriticalException
    );
}

TEST(RespPackageColumnDefinitionTest, ClientNot41)
{
    char                buffer[] =  
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x03\x00"      // len
                                    "\x03\x04\x05"      // columnLength
                                    "\xFC\x01\x00"      // len
                                    "\x07"              // type
                                    "\xFC\x02\x00"      // len
                                    "\x08"              // flags
                                    "\x0A"              // decimal
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(0, 0);

    RespPackageColumnDefinition      col(reader);
    EXPECT_TRUE(reader.isEmpty());

    EXPECT_EQ("", col.catalog);
    EXPECT_EQ("", col.schema);
    EXPECT_EQ("C", col.table);
    EXPECT_EQ("", col.orgTable);
    EXPECT_EQ("E", col.name);
    EXPECT_EQ("", col.orgName);
    EXPECT_EQ(0x00, col.lengthOfFixedField);
    EXPECT_EQ(0x00, col.charSet);
    EXPECT_EQ(0x050403, col.columnLength);
    EXPECT_EQ(0x07, col.type);
    EXPECT_EQ(0x08, col.flags);
    EXPECT_EQ(0x0A, col.decimal);
    EXPECT_EQ(0x00, col.filler);
    EXPECT_EQ(0, col.defaultValues.size());

    std::stringstream message;
    message << col;
    EXPECT_NE(message.str(), "");
}
TEST(RespPackageColumnDefinitionTest, ClientNot41LONGFLAG)
{
    char                buffer[] =  
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x03\x00"      // len
                                    "\x03\x04\x05"      // columnLength
                                    "\xFC\x01\x00"      // len
                                    "\x07"              // type
                                    "\xFC\x03\x00"      // len
                                    "\x09\x08"          // flags
                                    "\x0A"              // decimal
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_LONG_FLAG, 0);

    RespPackageColumnDefinition      col(reader);
    EXPECT_TRUE(reader.isEmpty());

    EXPECT_EQ("", col.catalog);
    EXPECT_EQ("", col.schema);
    EXPECT_EQ("C", col.table);
    EXPECT_EQ("", col.orgTable);
    EXPECT_EQ("E", col.name);
    EXPECT_EQ("", col.orgName);
    EXPECT_EQ(0x00, col.lengthOfFixedField);
    EXPECT_EQ(0x00, col.charSet);
    EXPECT_EQ(0x050403, col.columnLength);
    EXPECT_EQ(0x07, col.type);
    EXPECT_EQ(0x0809, col.flags);
    EXPECT_EQ(0x0A, col.decimal);
    EXPECT_EQ(0x00, col.filler);
    EXPECT_EQ(0, col.defaultValues.size());

    std::stringstream message;
    message << col;
    EXPECT_NE(message.str(), "");
}
TEST(RespPackageColumnDefinitionTest, ClientNot41LONGFLAGBadLen1Not3)
{
    char                buffer[] =  
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x04\x00"      // len
                                    "\x03\x04\x05"      // columnLength
                                    "\xFC\x01\x00"      // len
                                    "\x07"              // type
                                    "\xFC\x03\x00"      // len
                                    "\x09\x08"          // flags
                                    "\x0A"              // decimal
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_LONG_FLAG, 0);

    EXPECT_THROW(
        RespPackageColumnDefinition      col(reader),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(RespPackageColumnDefinitionTest, ClientNot41LONGFLAGBadLen2Not1)
{
    char                buffer[] =  
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x03\x00"      // len
                                    "\x03\x04\x05"      // columnLength
                                    "\xFC\x02\x00"      // len
                                    "\x07"              // type
                                    "\xFC\x03\x00"      // len
                                    "\x09\x08"          // flags
                                    "\x0A"              // decimal
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_LONG_FLAG, 0);

    EXPECT_THROW(
        RespPackageColumnDefinition      col(reader),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(RespPackageColumnDefinitionTest, ClientNot41LONGFLAGBadLen3Not3)
{
    char                buffer[] =  
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x03\x00"      // len
                                    "\x03\x04\x05"      // columnLength
                                    "\xFC\x01\x00"      // len
                                    "\x07"              // type
                                    "\xFC\x0A\x00"      // len
                                    "\x09\x08"          // flags
                                    "\x0A"              // decimal
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_LONG_FLAG, 0);

    EXPECT_THROW(
        RespPackageColumnDefinition      col(reader),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(RespPackageColumnDefinitionTest, ClientNot41BadLen3Not2)
{
    char                buffer[] =  
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x03\x00"      // len
                                    "\x03\x04\x05"      // columnLength
                                    "\xFC\x01\x00"      // len
                                    "\x07"              // type
                                    "\xFC\x0A\x00"      // len
                                    "\x08"              // flags
                                    "\x0A"              // decimal
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(0, 0);

    EXPECT_THROW(
        RespPackageColumnDefinition      col(reader),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(RespPackageColumnDefinitionTest, Client41DefaultValues)
{
    char                buffer[] =  "\xFC\x01\x00" "A"  // catalog
                                    "\xFC\x01\x00" "B"  // schema
                                    "\xFC\x01\x00" "C"  // table
                                    "\xFC\x01\x00" "D"  // orgTable
                                    "\xFC\x01\x00" "E"  // name
                                    "\xFC\x01\x00" "F"  // orgName
                                    "\xFC\x0C\x00"      // lengthOfFixedField
                                    "\x01\x02"          // charSet
                                    "\x03\x04\x05\x06"  // columnLength
                                    "\x07"              // type
                                    "\x08\x09"          // flags
                                    "\x0A"              // decimal
                                    "\x00\x00"          // filler
                                    "\xFC\x02\x00"      // DefaultValueLen
                                    "\xFC\x01\x00" "G"
                                    "\xFC\x02\x00" "HI"
                                    ;
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageColumnDefinition      col(reader, true);
    EXPECT_TRUE(reader.isEmpty());

    EXPECT_EQ("A", col.catalog);
    EXPECT_EQ("B", col.schema);
    EXPECT_EQ("C", col.table);
    EXPECT_EQ("D", col.orgTable);
    EXPECT_EQ("E", col.name);
    EXPECT_EQ("F", col.orgName);
    EXPECT_EQ(0x0C, col.lengthOfFixedField);
    EXPECT_EQ(0x0201, col.charSet);
    EXPECT_EQ(0x06050403, col.columnLength);
    EXPECT_EQ(0x07, col.type);
    EXPECT_EQ(0x0908, col.flags);
    EXPECT_EQ(0x0A, col.decimal);
    EXPECT_EQ(0x00, col.filler);
    ASSERT_EQ(2, col.defaultValues.size());
    EXPECT_EQ("G",  col.defaultValues[0]);
    EXPECT_EQ("HI", col.defaultValues[1]);

    std::stringstream message;
    message << col;
    EXPECT_NE(message.str(), "");
}


