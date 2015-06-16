
#include "Connection.h"

#include "gtest/gtest.h"


/*
 * Tests assume mysql is already up and running.
 * Default port 3306
 * Has a user called root with password testPassword
 */
TEST(ConnectionTest, CreateMySQLOnGeneric)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://127.0.0.1", "root", "testPassword", options);
}
TEST(ConnectionTest, Create)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    MySQL::Connection     connection("127.0.0.1", 3306, "root", "testPassword", options);
}


