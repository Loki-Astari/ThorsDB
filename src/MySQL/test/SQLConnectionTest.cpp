
#include "SQLConnection.h"

#include "gtest/gtest.h"

TEST(SQLConnectionTest, Create)
{
    using ThorsAnvil::SQL::SQLConnection;
    SQLConnection     connection("mysql://127.0.0.1:69", "root", "testPassword");
}


