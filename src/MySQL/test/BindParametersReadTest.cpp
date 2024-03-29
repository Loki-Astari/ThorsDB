
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersReadTest, BindFloatTestAgainstDouble)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE R1=?");
    long count = 0;
    float rbind = 10;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindFloatTestAgainstFloat)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE R2=?");
    long count = 0;
    float rbind = 11;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindFloatTestAgainstReal)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE R3=?");
    long count = 0;
    float rbind = 14;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindFloatTestAgainstDecimal)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE D1=?");
    long count = 0;
    float rbind = 12;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindFloatTestAgainstNumeric)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE D2=?");
    long count = 0;
    float rbind = 13;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindDoubleTestAgainstDouble)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE R1=?");
    long count = 0;
    double rbind = 10;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}
TEST(BindParametersReadTest, BindDoubleTestAgainstReal)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE R3=?");
    long count = 0;
    double rbind = 14;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindDoubleTestAgainstFloat)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE R2=?");
    long count = 0;
    double rbind = 11;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindDoubleTestAgainstDecimal)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE D1=?");
    long count = 0;
    double rbind = 12;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersReadTest, BindDoubleTestAgainstNumeric)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM RealTypes WHERE D2=?");
    long count = 0;
    double rbind = 13;
    statement.execute(DB::Access::Bind(rbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

