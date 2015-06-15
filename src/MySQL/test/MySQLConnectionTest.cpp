
#include "MySQLConnection.h"

#include "gtest/gtest.h"

TEST(MySQLConnectionTest, Create)
{
    using ThorsAnvil::MySQL::MySQLConnection;
    std::map<std::string, std::string>      options;
    MySQLConnection     connection("127.0.0.1", 69, "root", "testPassword", options);
}


