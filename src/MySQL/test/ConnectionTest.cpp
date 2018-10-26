
#include "Connection.h"
#include "DefaultMySQLConnection.h"
#include "ThorSQL/Connection.h"
#include "ThorSQL/StreamSimple.h"
#include "ConectReader.h"
#include "PackageBuffer.h"
#include "ThorSQL/StreamInterface.h"

#include "gtest/gtest.h"
#include "MySQLConfig.h"
#include "test/TableTest.h"

using namespace ThorsAnvil::MySQL;

ThorsAnvil::SQL::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlTestConnection("mysql");

/*
 * Tests assume mysql is already up and running.
 * Default port 3306
 * Has a user called test with password testPassword
 */
TEST(ConnectionTest, TryLocalMySQLConnection)
{
    using namespace ThorsAnvil;
    using Buffer=MySQL::PackageBuffer;
    std::map<std::string, std::string>      options;

    SQL::StreamSimple       stream("127.0.0.1", 0);
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

TEST(ConnectionTest, MySQLNativePassword)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}

TEST(ConnectionTest, CachingSHA2PasswordEmpty)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    "ssluserEmpty",
                                    "",
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}
TEST(ConnectionTest, CachingSHA2Password)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    "ssluser",
                                    "sslPassword",
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}


