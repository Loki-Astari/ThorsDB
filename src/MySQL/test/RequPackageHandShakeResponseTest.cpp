
#include "RequPackageHandShakeResp.h"
#include "RespPackageHandShake.h"
#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "Authentication.h"
#include "ThorSQL/StreamInterface.h"
#include "MySQLConfig.h"
#include "test/MockStream.h"
#include <algorithm>

#include "gtest/gtest.h"

using ThorsAnvil::MySQL::Authetication;
//
// The default capabilities in the following tests are:
//      capabilities    "\x06\x07"
//      cap2            "\x0B\x00"
//  This generates a capabilities.
//      capabilities    Hex:    0x0C0B8706
//                      Bin:    0000 0000 0000 1011 1000 0111 0000 0110
//
//                  Byte 1  0110
//                      CLIENT_FOUND_ROWS                       0x00000002
//                      CLIENT_LONG_FLAG                        0x00000004
//                  Byte 2  0000
//                  Byte 3  0111
//                      CLIENT_IGNORE_SPACE                     0x00000100
//                      CLIENT_PROTOCOL_41                      0x00000200
//                      CLIENT_INTERACTIVE                      0x00000400
//                  Byte 4  1000
//                      CLIENT_SECURE_CONNECTION                0x00008000
//                  Byte 5  1011
//                      CLIENT_MULTI_STATEMENTS                 0x00010000
//                      CLIENT_MULTI_RESULTS                    0x00020000
//                      CLIENT_PLUGIN_AUTH                      0x00080000
//                  Byte 6  0000
//                  Byte 7  0000

static const std::string testUser = THOR_TESTING_MYSQL_USER;
static const std::string testPass = THOR_TESTING_MYSQL_PASS;
static const std::string testDaBa = THOR_TESTING_MYSQL_DB;
static const ThorsAnvil::MySQL::Options testOptions;

TEST(RequPackageHandShakeResponseTest, Check_mysql_old_password)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x07"              // capabilities
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        "1234567890ABC"         // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "mysql_old_password"    // authPluginName  (Null Terminated)
                        ;

    MockStream                                 buffer(data, sizeof(data));
    ThorsAnvil::MySQL::ConectReader            reader(buffer);
    ThorsAnvil::MySQL::ConectWriter            writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake    handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection              connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);

    bool    didThrow = false;
    try {
        std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
        std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
        ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                            handShake.getCapabilities(),
                                                                            auth->getPluginName(),
                                                                            authResponse);
    }
    catch(std::runtime_error const& e) {
        didThrow    = true;
        std::string const&  msg     = e.what();
        std::size_t         find    = msg.find("mysql_old_password");
        ASSERT_NE(std::string::npos, find);
    }
    catch(...) {
    }
    ASSERT_NE(false, didThrow);
}
TEST(RequPackageHandShakeResponseTest, Check_mysql_clear_password)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x07"              // capabilities
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        "1234567890ABC"         // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "mysql_clear_password"  // authPluginName  (Null Terminated)
                        ;

    MockStream                                  buffer(data, sizeof(data));
    ThorsAnvil::MySQL::ConectReader             reader(buffer);
    ThorsAnvil::MySQL::ConectWriter            writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake     handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection              connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);

    bool    didThrow = false;
    try {
        std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
        std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
        ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                            handShake.getCapabilities(),
                                                                            auth->getPluginName(),
                                                                            authResponse);
    }
    catch(std::runtime_error const& e) {
        didThrow    = true;
        std::string const&  msg     = e.what();
        std::size_t         find    = msg.find("mysql_clear_password");
        ASSERT_NE(std::string::npos, find);
    }
    catch(...) {
    }
    ASSERT_NE(false, didThrow);
}

TEST(RequPackageHandShakeResponseTest, Check_authentication_windows_client)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x07"              // capabilities
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        "1234567890ABC"         // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "authentication_windows_client"    // authPluginName  (Null Terminated)
                        ;

    MockStream                                 buffer(data, sizeof(data));
    ThorsAnvil::MySQL::ConectReader            reader(buffer);
    ThorsAnvil::MySQL::ConectWriter            writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake    handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection              connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);

    bool    didThrow = false;
    try {
        std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
        std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
        ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                            handShake.getCapabilities(),
                                                                            auth->getPluginName(),
                                                                            authResponse);
    }
    catch(std::runtime_error const& e) {
        didThrow    = true;
        std::string const&  msg     = e.what();
        std::size_t         find    = msg.find("authentication_windows_client");
        ASSERT_NE(std::string::npos, find);
    }
    catch(...) {
    }
    ASSERT_NE(false, didThrow);
}

TEST(RequPackageHandShakeResponseTest, Check_sha256_password)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x07"              // capabilities
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        "1234567890ABC"         // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "sha256_password"       // authPluginName  (Null Terminated)
                        ;

    MockStream                                 buffer(data, sizeof(data));
    ThorsAnvil::MySQL::ConectReader            reader(buffer);
    ThorsAnvil::MySQL::ConectWriter            writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake    handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection              connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);

    bool    didThrow = false;
    try {
        std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
        std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
        ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                            handShake.getCapabilities(),
                                                                            auth->getPluginName(),
                                                                            authResponse);
    }
    catch(std::runtime_error const& e) {
        didThrow    = true;
        std::string const&  msg     = e.what();
        std::size_t         find    = msg.find("sha256_password");
        ASSERT_NE(std::string::npos, find);
    }
    catch(...) {
    }
    ASSERT_NE(false, didThrow);
}

TEST(RequPackageHandShakeResponseTest, Check_mysql_native_password)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x87"              // capabilities
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        "1234567890ABC"         // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "mysql_native_password"    // authPluginName  (Null Terminated)
                        ;

    MockStream                                 buffer(data, sizeof(data));
    ThorsAnvil::MySQL::ConectReader            reader(buffer);
    ThorsAnvil::MySQL::ConectWriter            writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake    handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection              connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);

    std::unique_ptr<Authetication> auth;
    ASSERT_NO_THROW(
        auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
    );
    // MIY CHECK
    std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
    ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                      handShake.getCapabilities(),
                                                                      auth->getPluginName(),
                                                                      authResponse);

    EXPECT_TRUE(buffer.isEmpty());
}

TEST(RequPackageHandShakeResponseTest, Check_unknown_type)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x07"              // capabilities
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        "1234567890ABC"         // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "Unknown"               // authPluginName  (Null Terminated)
                        ;

    MockStream                                 buffer(data, sizeof(data));
    ThorsAnvil::MySQL::ConectReader            reader(buffer);
    ThorsAnvil::MySQL::ConectWriter            writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake    handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection              connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);

    bool    didThrow = false;
    try {
        std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
        std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
        ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                            handShake.getCapabilities(),
                                                                            auth->getPluginName(),
                                                                            authResponse);
    }
    catch(std::runtime_error const& e) {
        didThrow    = true;
        std::string const&  msg     = e.what();
        std::size_t         find    = msg.find("UNKNOWN");
        ASSERT_NE(std::string::npos, find);
    }
    catch(...) {
    }
    ASSERT_NE(false, didThrow);
}

TEST(RequPackageHandShakeResponseTest, UsingSecureConnection)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x87"              // capabilities
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        "1234567890ABC"         // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "mysql_native_password"    // authPluginName  (Null Terminated)
                        ;

    unsigned char output[500];
    MockStream                                    buffer(data, sizeof(data), output);
    ThorsAnvil::MySQL::ConectReader               reader(buffer);
    ThorsAnvil::MySQL::ConectWriter               writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake       handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection                 connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);
    EXPECT_TRUE(buffer.isEmpty());

    std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
    std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
    ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                      handShake.getCapabilities(),
                                                                      auth->getPluginName(),
                                                                      authResponse);
    std::cerr << handShakeResp << "\n\n";

    std::stringstream message;
    message << handShakeResp;
    auto findusername = message.str().find("username(test)");
    EXPECT_NE(std::string::npos, findusername);
    auto findauthResponse = message.str().find("authResponse(0x62 0xa7 0x7f 0x30 0x28 0x50 0x3c 0x1f 0x24 0x71 0xa2 0xe2 0xa2 0xe6 0x97 0x9c 0xf9 0x58 0xc4 0x56 )");
    EXPECT_NE(std::string::npos, findauthResponse);
    auto findoptions = message.str().find("options()");
    EXPECT_NE(std::string::npos, findoptions);
    auto finddatabase = message.str().find("database(test)");
    EXPECT_NE(std::string::npos, finddatabase);
    auto findauthPluginName = message.str().find("authPluginName(mysql_native_password)");
    EXPECT_NE(std::string::npos, findauthPluginName);
    auto findcapabilities = message.str().find("capabilities(755204)");
    EXPECT_NE(std::string::npos, findcapabilities);
    auto findhumanMessage = message.str().find("humanMessage(RequPackageHandShakeResponse)");
    EXPECT_NE(std::string::npos, findhumanMessage);

    ThorsAnvil::MySQL::ConectWriter               writer(buffer);
    handShakeResp.build(writer);

    unsigned char result[] =
                    "\x04\x86\x0b\x00"
                    "\x00\x00\x00\x01"
                    "\x21"
                    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                    "\x74\x65\x73\x74\x00"
                    "\x14"
                    "\x62\xa7\x7f\x30\x28\x50\x3c\x1f\x24\x71\xa2\xe2\xa2\xe6\x97\x9c\xf9\x58\xc4\x56"
                    "\x6d\x79\x73\x71\x6c\x5f\x6e\x61\x74\x69\x76\x65\x5f\x70\x61\x73\x73\x77\x6f\x72\x64\x00";
    ASSERT_EQ(80, buffer.writLen());
    for(int loop = 0; loop < buffer.writLen(); ++loop)
    {
        EXPECT_EQ(result[loop], output[loop]);
    }

}
TEST(RequPackageHandShakeResponseTest, UsingNoSecureConnection)
{
    // capabilities = capabilities | (cap2 << 16)
    // authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
    char const data[]
                      = "FakeServer\0"          // serverVersion
                        "\x01\x02\x03\x04"      // Connection ID
                        "12345678"              // authPluginData       initial 8 characters.
                        "\x05"                  // check
                        "\x06\x07"              // capabilities  REMOVED CLIENT_SECURE_CONNECTION from previous test.
                        "\x08"                  // charset
                        "\x09\x0A"              // statusFlag
                        "\x0B\x00"              // cap2
                        "\x08"                  // authPluginLength
                        "ABCDEFGHIJ"            // reserved
                        ""                      // authPluginData2 (Length => std::max(13, authPluginLength - 8))
                        "mysql_native_password"    // authPluginName  (Null Terminated)
                        ;

    unsigned char output[500];
    MockStream                                    buffer(data, sizeof(data), output);
    ThorsAnvil::MySQL::ConectReader               reader(buffer);
    ThorsAnvil::MySQL::ConectWriter               writter(buffer);
    ThorsAnvil::MySQL::RespPackageHandShake       handShake(0x0A, reader);
    ThorsAnvil::MySQL::Connection                 connection(THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB, ThorsAnvil::MySQL::Options(),reader, writter);

    std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
    std::string authResponse = auth->getAuthenticationString(testUser, testPass, testDaBa, handShake.getAuthPluginData());
    ThorsAnvil::MySQL::RequPackageHandShakeResponse     handShakeResp(testUser, testPass, testOptions, testDaBa,
                                                                      handShake.getCapabilities(),
                                                                      auth->getPluginName(),
                                                                      authResponse);
    EXPECT_TRUE(buffer.isEmpty());

    ThorsAnvil::MySQL::ConectWriter               writer(buffer);
    handShakeResp.build(writer);

    unsigned char result[] =
                    "\x04\x06\x0b\x00"
                    "\x00\x00\x00\x01"
                    "\x21"
                    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                    "\x74\x65\x73\x74\x00"
                    "\x62\xa7\x7f\x30\x28\x50\x3c\x1f\x24\x71\xa2\xe2\xa2\xe6\x97\x9c\xf9\x58\xc4\x56\x00"
                    "\x6d\x79\x73\x71\x6c\x5f\x6e\x61\x74\x69\x76\x65\x5f\x70\x61\x73\x73\x77\x6f\x72\x64\x00";

    ASSERT_EQ(80, buffer.writLen());
    for(int loop = 0; loop < buffer.writLen(); ++loop)
    {
        EXPECT_EQ(result[loop], output[loop]);
    }

    std::stringstream message;
    message << handShakeResp;
    auto findusername = message.str().find("username(test)");
    EXPECT_NE(std::string::npos, findusername);
    auto findauthResponse = message.str().find("authResponse(0x62 0xa7 0x7f 0x30 0x28 0x50 0x3c 0x1f 0x24 0x71 0xa2 0xe2 0xa2 0xe6 0x97 0x9c 0xf9 0x58 0xc4 0x56 )");
    EXPECT_NE(std::string::npos, findauthResponse);
    auto findoptions = message.str().find("options()");
    EXPECT_NE(std::string::npos, findoptions);
    auto finddatabase = message.str().find("database(test)");
    EXPECT_NE(std::string::npos, finddatabase);
    auto findauthPluginName = message.str().find("authPluginName(mysql_native_password)");
    EXPECT_NE(std::string::npos, findauthPluginName);
    auto findcapabilities = message.str().find("capabilities(722436)");
    EXPECT_NE(std::string::npos, findcapabilities);
    auto findhumanMessage = message.str().find("humanMessage(RequPackageHandShakeResponse)");
    EXPECT_NE(std::string::npos, findhumanMessage);
}



