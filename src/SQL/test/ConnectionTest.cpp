
#include "Connection.h"

#include "gtest/gtest.h"

class MockConectionProxyRegister: public ThorsAnvil::SQL::ConnectionProxy
{
    public:
        MockConectionProxyRegister(std::string const&, int, std::string const&, std::string const&, std::string const&, ThorsAnvil::SQL::Options const&)
        {
        }
};

ThorsAnvil::SQL::ConnectionCreatorRegister<MockConectionProxyRegister>   registerFakeMysql("mysql");

TEST(ConnectionTest, Create)
{
    using ThorsAnvil::SQL::Connection;
    Connection     connection("mysql://127.0.0.1:69", "root", "testPassword", "test");
}
TEST(ConnectionTest, CreateDefaultPort)
{
    using ThorsAnvil::SQL::Connection;
    Connection     connection("mysql://127.0.0.1", "root", "testPassword", "test");
}

TEST(ConnectionTest, BadSchema)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection("badschema://127.0.0.1:69", "root", "testPassword", "test"),
    std::runtime_error
    );
}

TEST(ConnectionTest, NoSchema)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection("127.0.0.1:69", "root", "testPassword", "test"),
    std::runtime_error
    );
}
TEST(ConnectionTest, BadHost)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection("mysql://:69", "root", "testPassword", "test"),
    std::runtime_error
    );
}
TEST(ConnectionTest, BadPort)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection("mysql://127.0.0.1:XY", "root", "testPassword", "test"),
    std::runtime_error
    );
}

