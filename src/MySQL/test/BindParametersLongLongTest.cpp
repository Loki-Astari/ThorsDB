
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit4)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit12)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit24)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit48)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUTinyInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstTinyInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUSmallInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstSmallInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUMedInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstMedInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstULongInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstLongInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstULongLongInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstLongLongInt)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

