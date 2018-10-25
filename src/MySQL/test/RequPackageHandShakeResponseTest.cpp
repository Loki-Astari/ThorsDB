
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
                        "\x0B\x0C"              // cap2
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
                        "\x0B\x0C"              // cap2
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
                        "\x0B\x0C"              // cap2
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
                        "\x0B\x0C"              // cap2
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
                        "\x0B\x0C"              // cap2
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

    ASSERT_NO_THROW(
        std::unique_ptr<Authetication> auth = ThorsAnvil::MySQL::getAuthenticatonMethod(connection, handShake.getAuthPluginName());
    );
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
                        "\x0B\x0C"              // cap2
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

