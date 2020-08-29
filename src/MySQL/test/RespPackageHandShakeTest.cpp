
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../ThorsDB/Connection.h"
#include "ConectReader.h"
#include "RespPackageHandShake.h"
#include "test/MockStream.h"

using ThorsAnvil::DB::SQL::Options;
using ThorsAnvil::DB::MySQL::ConectReader;
using ThorsAnvil::DB::MySQL::RespPackageHandShake;

TEST(RespPackageHandShakeTest, DefaultHandShakeMessage)
{
    char                buffer[] =  "MockServer\x00"    // serverVersion
                                    "\x01\x02\x03\x04"  // connectionID
                                    "12345678"          // authPluginData
                                    "\x01"              // check
                                    "\x02\x83"          // capabilities  CLIENT_PLUGIN_AUTH | CLIENT_SECURE_CONNECTION | CLIENT_PLUGIN_AUTH = 0x00080000 | 0x00008000 = 0x00088000
                                    "\x04"              // charset
                                    "\x05\x06"          // statusFlag
                                    "\x08\x07"          // cap2
                                    "\x09"              // authPluginLength
                                    "\x12\x23\x34\x45\x56\x67\x78\x89\x9A\xAB"  // Reserved must by all 0
                                    "123456789ABCD" // authPluginData2
                                    "PluginName\x00"    // authPluginName
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageHandShake      handShake(0x0A, reader);

    EXPECT_FALSE(handShake.isOK());
    EXPECT_FALSE(handShake.isError());
    EXPECT_FALSE(handShake.isEOF());
    EXPECT_TRUE(handShake.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << handShake;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    EXPECT_EQ(0x07088302, handShake.getCapabilities());
    EXPECT_EQ(0x04, handShake.getCharset());
    EXPECT_EQ("PluginName", handShake.getAuthPluginName());
    EXPECT_EQ("12345678", handShake.getAuthPluginData());

    auto findServerVersion = message.str().find("serverVersion(MockServer)");
    EXPECT_NE(std::string::npos, findServerVersion);
    auto findConnectionID = message.str().find("connectionID(0x04030201)");
    EXPECT_NE(std::string::npos, findConnectionID);
    auto findAuthPluginData = message.str().find("authPluginData(0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 )"); // 12345678
    EXPECT_NE(std::string::npos, findAuthPluginData);
    auto findCheck = message.str().find("check(1)");
    EXPECT_NE(std::string::npos, findCheck);
    auto findAuthPluginLength = message.str().find("authPluginLength(9)");
    EXPECT_NE(std::string::npos, findAuthPluginLength);
    auto findReserved = message.str().find("reserved(0x12 0x23 0x34 0x45 0x56 0x67 0x78 0x89 0x9a 0xab )");
    EXPECT_NE(std::string::npos, findReserved);
    auto findAuthPluginName = message.str().find("authPluginName(PluginName)");
    EXPECT_NE(std::string::npos, findAuthPluginName);
    auto findIsV9 = message.str().find("isV9(0)");
    EXPECT_NE(std::string::npos, findIsV9);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageHandShakeTest, V9Handshake)
{
    char                buffer[] =  "MockServer\x00"    // serverVersion
                                    "\x01\x02\x03\x04"  // connectionID
                                    "12345678"          // authPluginData
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageHandShake      handShake(0x0A, reader);

    EXPECT_FALSE(handShake.isOK());
    EXPECT_FALSE(handShake.isError());
    EXPECT_FALSE(handShake.isEOF());
    EXPECT_TRUE(handShake.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << handShake;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    EXPECT_EQ(0x00000000, handShake.getCapabilities());
    EXPECT_EQ(0x00, handShake.getCharset());
    EXPECT_EQ("", handShake.getAuthPluginName());
    EXPECT_EQ("12345678", handShake.getAuthPluginData());

    auto findServerVersion = message.str().find("serverVersion(MockServer)");
    EXPECT_NE(std::string::npos, findServerVersion);
    auto findConnectionID = message.str().find("connectionID(0x04030201)");
    EXPECT_NE(std::string::npos, findConnectionID);
    auto findAuthPluginData = message.str().find("authPluginData(0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 )"); // 12345678
    EXPECT_NE(std::string::npos, findAuthPluginData);
    auto findCheck = message.str().find("check(0)");
    EXPECT_NE(std::string::npos, findCheck);
    auto findAuthPluginLength = message.str().find("authPluginLength(0)");
    EXPECT_NE(std::string::npos, findAuthPluginLength);
    auto findReserved = message.str().find("reserved()");
    EXPECT_NE(std::string::npos, findReserved);
    auto findAuthPluginName = message.str().find("authPluginName()");
    EXPECT_NE(std::string::npos, findAuthPluginName);
    auto findIsV9 = message.str().find("isV9(1)");
    EXPECT_NE(std::string::npos, findIsV9);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageHandShakeTest, SmallMessage)
{
    char                buffer[] =  "MockServer\x00"    // serverVersion
                                    "\x01\x02\x03\x04"  // connectionID
                                    "12345678"          // authPluginData
                                    "\x01"              // check
                                    "\x02\x83"          // capabilities  CLIENT_PLUGIN_AUTH | CLIENT_SECURE_CONNECTION | CLIENT_PLUGIN_AUTH = 0x00080000 | 0x00008000 = 0x00088000
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageHandShake      handShake(0x0A, reader);

    EXPECT_FALSE(handShake.isOK());
    EXPECT_FALSE(handShake.isError());
    EXPECT_FALSE(handShake.isEOF());
    EXPECT_TRUE(handShake.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << handShake;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    EXPECT_EQ(0x00008302, handShake.getCapabilities());
    EXPECT_EQ(0x00, handShake.getCharset());
    EXPECT_EQ("", handShake.getAuthPluginName());
    EXPECT_EQ("12345678", handShake.getAuthPluginData());

    auto findServerVersion = message.str().find("serverVersion(MockServer)");
    EXPECT_NE(std::string::npos, findServerVersion);
    auto findConnectionID = message.str().find("connectionID(0x04030201)");
    EXPECT_NE(std::string::npos, findConnectionID);
    auto findAuthPluginData = message.str().find("authPluginData(0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 )"); // 12345678
    EXPECT_NE(std::string::npos, findAuthPluginData);
    auto findCheck = message.str().find("check(1)");
    EXPECT_NE(std::string::npos, findCheck);
    auto findAuthPluginLength = message.str().find("authPluginLength(0)");
    EXPECT_NE(std::string::npos, findAuthPluginLength);
    auto findReserved = message.str().find("reserved()");
    EXPECT_NE(std::string::npos, findReserved);
    auto findAuthPluginName = message.str().find("authPluginName()");
    EXPECT_NE(std::string::npos, findAuthPluginName);
    auto findIsV9 = message.str().find("isV9(0)");
    EXPECT_NE(std::string::npos, findIsV9);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageHandShakeTest, HandShakeNOAuth)
{
    char                buffer[] =  "MockServer\x00"    // serverVersion
                                    "\x01\x02\x03\x04"  // connectionID
                                    "12345678"          // authPluginData
                                    "\x01"              // check
                                    "\x02\x83"          // capabilities  CLIENT_SECURE_CONNECTION = 0x00008000 = 0x00008000
                                    "\x04"              // charset
                                    "\x05\x06"          // statusFlag
                                    "\x07\x07"          // cap2
                                    "\x00"              // fill
                                    "\x12\x23\x34\x45\x56\x67\x78\x89\x9A\xAB"  // Reserved must by all 0
                                    "123456789ABCD"     // authPluginData2
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageHandShake      handShake(0x0A, reader);

    EXPECT_FALSE(handShake.isOK());
    EXPECT_FALSE(handShake.isError());
    EXPECT_FALSE(handShake.isEOF());
    EXPECT_TRUE(handShake.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << handShake;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    EXPECT_EQ(0x07078302, handShake.getCapabilities());
    EXPECT_EQ(0x04, handShake.getCharset());
    EXPECT_EQ("", handShake.getAuthPluginName());
    EXPECT_EQ("12345678", handShake.getAuthPluginData());

    auto findServerVersion = message.str().find("serverVersion(MockServer)");
    EXPECT_NE(std::string::npos, findServerVersion);
    auto findConnectionID = message.str().find("connectionID(0x04030201)");
    EXPECT_NE(std::string::npos, findConnectionID);
    auto findAuthPluginData = message.str().find("authPluginData(0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 )"); // 12345678
    EXPECT_NE(std::string::npos, findAuthPluginData);
    auto findCheck = message.str().find("check(1)");
    EXPECT_NE(std::string::npos, findCheck);
    auto findAuthPluginLength = message.str().find("authPluginLength(9)");
    EXPECT_NE(std::string::npos, findAuthPluginLength);
    auto findReserved = message.str().find("reserved(0x12 0x23 0x34 0x45 0x56 0x67 0x78 0x89 0x9a 0xab )");
    EXPECT_NE(std::string::npos, findReserved);
    auto findAuthPluginName = message.str().find("authPluginName()");
    EXPECT_NE(std::string::npos, findAuthPluginName);
    auto findIsV9 = message.str().find("isV9(0)");
    EXPECT_NE(std::string::npos, findIsV9);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageHandShakeTest, HandShakeNOConnect)
{
    char                buffer[] =  "MockServer\x00"    // serverVersion
                                    "\x01\x02\x03\x04"  // connectionID
                                    "12345678"          // authPluginData
                                    "\x01"              // check
                                    "\x02\x73"          // capabilities  CLIENT_PLUGIN_AUTH | CLIENT_PLUGIN_AUTH = 0x00080000 = 0x00080000
                                    "\x04"              // charset
                                    "\x05\x06"          // statusFlag
                                    "\x08\x07"          // cap2
                                    "\x09"              // authPluginLength
                                    "\x12\x23\x34\x45\x56\x67\x78\x89\x9A\xAB"  // Reserved must by all 0
                                    "PluginName\x00"    // authPluginName
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageHandShake      handShake(0x0A, reader);

    EXPECT_FALSE(handShake.isOK());
    EXPECT_FALSE(handShake.isError());
    EXPECT_FALSE(handShake.isEOF());
    EXPECT_TRUE(handShake.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << handShake;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    EXPECT_EQ(0x07087302, handShake.getCapabilities());
    EXPECT_EQ(0x04, handShake.getCharset());
    EXPECT_EQ("PluginName", handShake.getAuthPluginName());
    EXPECT_EQ("12345678", handShake.getAuthPluginData());

    auto findServerVersion = message.str().find("serverVersion(MockServer)");
    EXPECT_NE(std::string::npos, findServerVersion);
    auto findConnectionID = message.str().find("connectionID(0x04030201)");
    EXPECT_NE(std::string::npos, findConnectionID);
    auto findAuthPluginData = message.str().find("authPluginData(0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 )"); // 12345678
    EXPECT_NE(std::string::npos, findAuthPluginData);
    auto findCheck = message.str().find("check(1)");
    EXPECT_NE(std::string::npos, findCheck);
    auto findAuthPluginLength = message.str().find("authPluginLength(9)");
    EXPECT_NE(std::string::npos, findAuthPluginLength);
    auto findReserved = message.str().find("reserved(0x12 0x23 0x34 0x45 0x56 0x67 0x78 0x89 0x9a 0xab )");
    EXPECT_NE(std::string::npos, findReserved);
    auto findAuthPluginName = message.str().find("authPluginName(PluginName)");
    EXPECT_NE(std::string::npos, findAuthPluginName);
    auto findIsV9 = message.str().find("isV9(0)");
    EXPECT_NE(std::string::npos, findIsV9);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageHandShakeTest, NoExtraFlags)
{
    char                buffer[] =  "MockServer\x00"    // serverVersion
                                    "\x01\x02\x03\x04"  // connectionID
                                    "12345678"          // authPluginData
                                    "\x01"              // check
                                    "\x02\x73"          // capabilities
                                    "\x04"              // charset
                                    "\x05\x06"          // statusFlag
                                    "\x07\x07"          // cap2
                                    "\x00"              // authPluginLength
                                    "\x12\x23\x34\x45\x56\x67\x78\x89\x9A\xAB"  // Reserved must by all 0
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageHandShake      handShake(0x0A, reader);

    EXPECT_FALSE(handShake.isOK());
    EXPECT_FALSE(handShake.isError());
    EXPECT_FALSE(handShake.isEOF());
    EXPECT_TRUE(handShake.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << handShake;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    EXPECT_EQ(0x07077302, handShake.getCapabilities());
    EXPECT_EQ(0x04, handShake.getCharset());
    EXPECT_EQ("", handShake.getAuthPluginName());
    EXPECT_EQ("12345678", handShake.getAuthPluginData());

    auto findServerVersion = message.str().find("serverVersion(MockServer)");
    EXPECT_NE(std::string::npos, findServerVersion);
    auto findConnectionID = message.str().find("connectionID(0x04030201)");
    EXPECT_NE(std::string::npos, findConnectionID);
    auto findAuthPluginData = message.str().find("authPluginData(0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 )"); // 12345678
    EXPECT_NE(std::string::npos, findAuthPluginData);
    auto findCheck = message.str().find("check(1)");
    EXPECT_NE(std::string::npos, findCheck);
    auto findAuthPluginLength = message.str().find("authPluginLength(9)");
    EXPECT_NE(std::string::npos, findAuthPluginLength);
    auto findReserved = message.str().find("reserved(0x12 0x23 0x34 0x45 0x56 0x67 0x78 0x89 0x9a 0xab )");
    EXPECT_NE(std::string::npos, findReserved);
    auto findAuthPluginName = message.str().find("authPluginName()");
    EXPECT_NE(std::string::npos, findAuthPluginName);
    auto findIsV9 = message.str().find("isV9(0)");
    EXPECT_NE(std::string::npos, findIsV9);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageHandShakeTest, LongAuthData)
{
    char                buffer[] =  "MockServer\x00"    // serverVersion
                                    "\x01\x02\x03\x04"  // connectionID
                                    "12345678"          // authPluginData
                                    "\x01"              // check
                                    "\x02\x83"          // capabilities  CLIENT_PLUGIN_AUTH | CLIENT_SECURE_CONNECTION | CLIENT_PLUGIN_AUTH = 0x00080000 | 0x00008000 = 0x00088000
                                    "\x04"              // charset
                                    "\x05\x06"          // statusFlag
                                    "\x08\x07"          // cap2
                                    "\x16"              // authPluginLength
                                    "\x12\x23\x34\x45\x56\x67\x78\x89\x9A\xAB"  // Reserved must by all 0
                                    "123456789ABCD\x00" // authPluginData2
                                    "PluginName\x00"    // authPluginName
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageHandShake      handShake(0x0A, reader);

    EXPECT_FALSE(handShake.isOK());
    EXPECT_FALSE(handShake.isError());
    EXPECT_FALSE(handShake.isEOF());
    EXPECT_TRUE(handShake.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << handShake;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    EXPECT_EQ(0x07088302, handShake.getCapabilities());
    EXPECT_EQ(0x04, handShake.getCharset());
    EXPECT_EQ("PluginName", handShake.getAuthPluginName());
    EXPECT_EQ("12345678123456789ABCD", handShake.getAuthPluginData());

    auto findServerVersion = message.str().find("serverVersion(MockServer)");
    EXPECT_NE(std::string::npos, findServerVersion);
    auto findConnectionID = message.str().find("connectionID(0x04030201)");
    EXPECT_NE(std::string::npos, findConnectionID);
    auto findAuthPluginData = message.str().find("authPluginData(0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x41 0x42 0x43 0x44 )"); // 12345678123456789ABCD
    EXPECT_NE(std::string::npos, findAuthPluginData);
    auto findCheck = message.str().find("check(1)");
    EXPECT_NE(std::string::npos, findCheck);
    auto findAuthPluginLength = message.str().find("authPluginLength(22)");
    EXPECT_NE(std::string::npos, findAuthPluginLength);
    auto findReserved = message.str().find("reserved(0x12 0x23 0x34 0x45 0x56 0x67 0x78 0x89 0x9a 0xab )");
    EXPECT_NE(std::string::npos, findReserved);
    auto findAuthPluginName = message.str().find("authPluginName(PluginName)");
    EXPECT_NE(std::string::npos, findAuthPluginName);
    auto findIsV9 = message.str().find("isV9(0)");
    EXPECT_NE(std::string::npos, findIsV9);

    EXPECT_TRUE(reader.isEmpty());
}

