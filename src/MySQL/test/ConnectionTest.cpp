
#include "Connection.h"
#include "DefaultMySQLConnection.h"
#include "ThorsDB/Connection.h"
#include "ThorsDB/StreamSimple.h"
#include "ConectReader.h"
#include "PackageBuffer.h"
#include "ThorsDB/StreamInterface.h"

#include "gtest/gtest.h"
#include "MySQLConfig.h"
#include "test/TableTest.h"

using namespace ThorsAnvil::DB::MySQL;

ThorsAnvil::DB::SQL::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlTestConnection("mysql");

/*
 * Tests assume mysql is already up and running.
 * Default port 3306
 * Has a user called test with password testPassword
 */
TEST(ConnectionTest, CreateMySQLOnGeneric)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}

TEST(ConnectionTest, CreateMySQLOnGenericGetSocket)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
    EXPECT_NE(-1, connection.getSocketId());
}

TEST(ConnectionTest, CreateMySQLOnGenericSetYield)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    connection.setYield([](){}, [](){});
}

TEST(ConnectionTest, Create)
{
    using namespace ThorsAnvil;
    using Buffer=DB::MySQL::PackageBuffer;
    std::map<std::string, std::string>      options;

    DB::SQL::StreamSimple       stream("127.0.0.1", 0);
    Buffer                  buffer(stream, true);
    DB::MySQL::ConectReader     reader(buffer);
    DB::MySQL::ConectWriter     writer(buffer);
    DB::MySQL::Connection       connection(THOR_TESTING_MYSQL_USER,
                                       THOR_TESTING_MYSQL_PASS,
                                       THOR_TESTING_MYSQL_DB,
                                       options,
                                       reader,
                                       writer);
}

TEST(ConnectionTest, MySQLNativePassword)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}

#if MYSQL_MAJOR_VERSION >= 6
TEST(ConnectionTest, CachingSHA2PasswordEmpty)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    "ssluserEmpty",
                                    "",
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}
TEST(ConnectionTest, CachingSHA2Password)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    "ssluser",
                                    "sslPassword",
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}
#endif


