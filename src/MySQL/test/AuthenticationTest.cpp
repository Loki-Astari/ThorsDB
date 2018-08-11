
#include <gtest/gtest.h>
#include "Authentication.h"
#include "ThorSQL/SQLUtil.h"

using ThorsAnvil::SQL::Options;

TEST(AuthenticationTest, MySQLOldPassword)
{
    auto test = []()
    {
        ThorsAnvil::MySQL::authentication("mysql_old_password", "Plop", "test", "testPassword", Options{}, "test");
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
        ThorsAnvil::MySQL::authentication("mysql_clear_password", "Plop", "test", "testPassword", Options{}, "test");
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
        ThorsAnvil::MySQL::authentication("authentication_windows_client", "Plop", "test", "testPassword", Options{}, "test");
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
        ThorsAnvil::MySQL::authentication("sha256_password", "Plop", "test", "testPassword", Options{}, "test");
    };
    EXPECT_THROW(
        test(),
        std::runtime_error
    );
}
TEST(AuthenticationTest, MySQLCachingSHA2)
{
    auto test = []()
    {
        ThorsAnvil::MySQL::authentication("caching_sha2_password", "Plop", "test", "testPassword", Options{}, "test");
    };
    EXPECT_THROW(
        test(),
        std::runtime_error
    );
}
TEST(AuthenticationTest, MySQLNativePassword)
{
    std::string result = ThorsAnvil::MySQL::authentication("mysql_native_password", "Plop", "test", "testPassword", Options{}, "test");
    EXPECT_EQ("\x5b\xd1\x0c\x5a\x53\x54\x6c\xff\x41\xe2\xb8\xf0\x69\x05\xd5\x74\x0b\x16\xa5\x8f", result);
}

