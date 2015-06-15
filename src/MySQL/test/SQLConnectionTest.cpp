
#include "SQLConnection.h"

#include "gtest/gtest.h"

TEST(SQLConnectionTest, Create)
{
    using ThorsAnvil::SQL::SQLConnection;
    SQLConnection     connection("mysql://127.0.0.1:69", "root", "testPassword");
}
TEST(SQLConnectionTest, CreateDefaultPort)
{
    using ThorsAnvil::SQL::SQLConnection;
    SQLConnection     connection("mysql://127.0.0.1", "root", "testPassword");
}

TEST(SQLConnectionTest, BadSchema)
{
    using ThorsAnvil::SQL::SQLConnection;
    ASSERT_THROW(
    SQLConnection     connection("badschema://127.0.0.1:69", "root", "testPassword"),
    std::runtime_error
    );
}

TEST(SQLConnectionTest, NoSchema)
{
    using ThorsAnvil::SQL::SQLConnection;
    ASSERT_THROW(
    SQLConnection     connection("127.0.0.1:69", "root", "testPassword"),
    std::runtime_error
    );
}
TEST(SQLConnectionTest, BadHost)
{
    using ThorsAnvil::SQL::SQLConnection;
    ASSERT_THROW(
    SQLConnection     connection("mysql://:69", "root", "testPassword"),
    std::runtime_error
    );
}
TEST(SQLConnectionTest, BadPort)
{
    using ThorsAnvil::SQL::SQLConnection;
    ASSERT_THROW(
    SQLConnection     connection("mysql://127.0.0.1:XY", "root", "testPassword"),
    std::runtime_error
    );
}

