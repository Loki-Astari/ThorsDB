
#include <gtest/gtest.h>
#include "ThorSQL/SQLUtil.h"
#include "test/MockStream.h"
#include "Authentication.h"
#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"

using ThorsAnvil::DB::SQL::Options;

TEST(AuthenticationTest, MySQLOldPassword)
{
    auto test = []()
    {
        MockStream                          stream(nullptr,0);
        ThorsAnvil::DB::MySQL::ConectReader     reader(stream);
        ThorsAnvil::DB::MySQL::ConectWriter     writer(stream);
        ThorsAnvil::DB::MySQL::Connection       connection( THOR_TESTING_MYSQL_USER,
                                                        THOR_TESTING_MYSQL_PASS,
                                                        THOR_TESTING_MYSQL_DB,
                                                        Options{},
                                                        reader,
                                                        writer
                                                      );
        getAuthenticatonMethod(connection, "mysql_old_password", Options{});
    };
    EXPECT_THROW(
        test(),
        std::runtime_error
    );
}
TEST(AuthenticationTest, MySQLClearPassword)
{
    auto test = []()
    {
        MockStream                          stream(nullptr,0);
        ThorsAnvil::DB::MySQL::ConectReader     reader(stream);
        ThorsAnvil::DB::MySQL::ConectWriter     writer(stream);
        ThorsAnvil::DB::MySQL::Connection       connection( THOR_TESTING_MYSQL_USER,
                                                        THOR_TESTING_MYSQL_PASS,
                                                        THOR_TESTING_MYSQL_DB,
                                                        Options{},
                                                        reader,
                                                        writer
                                                      );
        getAuthenticatonMethod(connection, "mysql_clear_password", Options{});
    };
    EXPECT_THROW(
        test(),
        std::runtime_error
    );
}
TEST(AuthenticationTest, MySQLWindowsClient)
{
    auto test = []()
    {
        MockStream                          stream(nullptr,0);
        ThorsAnvil::DB::MySQL::ConectReader     reader(stream);
        ThorsAnvil::DB::MySQL::ConectWriter     writer(stream);
        ThorsAnvil::DB::MySQL::Connection       connection( THOR_TESTING_MYSQL_USER,
                                                        THOR_TESTING_MYSQL_PASS,
                                                        THOR_TESTING_MYSQL_DB,
                                                        Options{},
                                                        reader,
                                                        writer
                                                      );
        getAuthenticatonMethod(connection, "authentication_windows_client", Options{});
    };
    EXPECT_THROW(
        test(),
        std::runtime_error
    );
}
TEST(AuthenticationTest, MySQLSH256)
{
    auto test = []()
    {
        MockStream                          stream(nullptr,0);
        ThorsAnvil::DB::MySQL::ConectReader     reader(stream);
        ThorsAnvil::DB::MySQL::ConectWriter     writer(stream);
        ThorsAnvil::DB::MySQL::Connection       connection( THOR_TESTING_MYSQL_USER,
                                                        THOR_TESTING_MYSQL_PASS,
                                                        THOR_TESTING_MYSQL_DB,
                                                        Options{},
                                                        reader,
                                                        writer
                                                      );
        getAuthenticatonMethod(connection, "sha256_password", Options{});
    };
    EXPECT_THROW(
        test(),
        std::runtime_error
    );
}
TEST(AuthenticationTest, MySQLCachingSHA2)
{
    MockStream                          stream(nullptr,0);
    ThorsAnvil::DB::MySQL::ConectReader     reader(stream);
    ThorsAnvil::DB::MySQL::ConectWriter     writer(stream);
    ThorsAnvil::DB::MySQL::Connection       connection( THOR_TESTING_MYSQL_USER,
                                                    THOR_TESTING_MYSQL_PASS,
                                                    THOR_TESTING_MYSQL_DB,
                                                    Options{},
                                                    reader,
                                                    writer
                                                  );
    std::unique_ptr<ThorsAnvil::DB::MySQL::Authetication> auth = getAuthenticatonMethod(connection, "caching_sha2_password", Options{});
    std::string result = auth->getAuthenticationString("test", "testPassword", "test", "Plop");
    char data[] = "\x4B\x8A\x7D\x52\xAB\xC4\xE4\x64\xCD\x88\xD1\x3D\xAD\x60\xF3\x7D\x90\x1D\x68\xDC\xE7\xD5\x9A\x00\xA3\xAF\xC3\xFE\x5B\x4D\xF0\x1C";
    EXPECT_EQ(std::string(data, 32), result);
}
TEST(AuthenticationTest, MySQLNativePassword)
{
    MockStream                          stream(nullptr,0);
    ThorsAnvil::DB::MySQL::ConectReader     reader(stream);
    ThorsAnvil::DB::MySQL::ConectWriter     writer(stream);
    ThorsAnvil::DB::MySQL::Connection       connection( THOR_TESTING_MYSQL_USER,
                                                     THOR_TESTING_MYSQL_PASS,
                                                     THOR_TESTING_MYSQL_DB,
                                                     Options{},
                                                     reader,
                                                     writer
                                                  );
    std::unique_ptr<ThorsAnvil::DB::MySQL::Authetication> auth = getAuthenticatonMethod(connection, "mysql_native_password", Options{});
    std::string result = auth->getAuthenticationString("test", "testPassword", "test", "Plop");
    //std::string result = ThorsAnvil::DB::MySQL::authentication("mysql_native_password", "Plop", "test", "testPassword", Options{}, "test");
    EXPECT_EQ("\x5b\xd1\x0c\x5a\x53\x54\x6c\xff\x41\xe2\xb8\xf0\x69\x05\xd5\x74\x0b\x16\xa5\x8f", result);
}

