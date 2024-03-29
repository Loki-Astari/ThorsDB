
#include "Connection.h"
#include "DefaultMySQLConnection.h"
#include "ThorsDB/Connection.h"
#include "ThorsDBCommon/StreamSimple.h"
#include "ConectReader.h"
#include "PackageBuffer.h"
#include "ThorsDBCommon/StreamInterface.h"

#include "gtest/gtest.h"
#include "MySQLConfig.h"
#include "test/TableTest.h"

using namespace ThorsAnvil::DB::MySQL;

ThorsAnvil::DB::Access::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlTestConnection("mysql");

/*
 * Tests assume mysql is already up and running.
 * Default port 3306
 * Has a user called test with password testPassword
 */
TEST(ConnectionTest, CreateMySQLOnGeneric)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}

TEST(ConnectionTest, CreateMySQLOnGenericGetSocket)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
    EXPECT_NE(-1, connection.getSocketId());
}

TEST(ConnectionTest, CreateMySQLOnGenericSetYield)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    connection.setYield([](){}, [](){});
}

TEST(ConnectionTest, Create)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    using Buffer=DB::MySQL::PackageBuffer;
    std::map<std::string, std::string>      options;

    DB::Common::StreamSimple    stream("127.0.0.1", 0);
    Buffer                      buffer(stream, true);
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
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}

/*
 * To disable these test:
 *      --disable-mysql-test-server-version-below-5
 */
TEST(ConnectionTest, CachingSHA2PasswordEmpty)
{
    SocketSetUp     setupSockets;

#if THOR_DISABLE_MYSQL_TEST_SERVER_VERSION_BELOW_5
    GTEST_SKIP << "MySQL Server is below version 5. Thus does not support SHA2 Password";
#endif

    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    "ssluserEmpty",
                                    "",
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}
TEST(ConnectionTest, CachingSHA2Password)
{
    SocketSetUp     setupSockets;

#if THOR_DISABLE_MYSQL_TEST_SERVER_VERSION_BELOW_5
    GTEST_SKIP << "MySQL Server is below version 5. Thus does not support SHA2 Password";
#endif

    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    "ssluser",
                                    "sslPassword",
                                    THOR_TESTING_MYSQL_DB,
                                    options);
}


