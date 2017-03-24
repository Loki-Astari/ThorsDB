
#include "Connection.h"
#include "DefaultMySQLConnection.h"
#include "ThorSQL/Connection.h"
#include "ConectReader.h"
#include "PackageBuffer.h"
#include "ThorSQL/StreamInterface.h"

#include "gtest/gtest.h"
#include "MySQLConfig.h"

using namespace ThorsAnvil::MySQL;

ThorsAnvil::SQL::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlTestConnection("mysql");

/*
 * Tests assume mysql is already up and running.
 * Default port 3306
 * Has a user called test with password testPassword
 */
TEST(ConnectionTest, CreateMySQLOnGeneric)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}

TEST(ConnectionTest, Create)
{
    using namespace ThorsAnvil;
    using Buffer=MySQL::PackageBuffer<MySQL::MySQLStream>;
    std::map<std::string, std::string>      options;

    MySQL::MySQLStream      stream("127.0.0.1", 0);
    Buffer                  buffer(stream, true);
    MySQL::ConectReader     reader(buffer);
    MySQL::ConectWriter     writer(buffer);
    MySQL::Connection       connection(THOR_TESTING_MYSQL_USER,
                                       THOR_TESTING_MYSQL_PASS,
                                       THOR_TESTING_MYSQL_DB,
                                       options,
                                       reader,
                                       writer);
}


