
#include "Connection.h"

#include "gtest/gtest.h"

TEST(ConnectionTest, CreateMySQLOnGeneric)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://127.0.0.1:69", "root", "testPassword", options);
}
TEST(ConnectionTest, Create)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    MySQL::Connection     connection("127.0.0.1", 69, "root", "testPassword", options);
}


