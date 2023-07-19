
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersShortTest, BindShortTestAgainstBit4)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstBit12)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstBit24)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstBit48)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUTinyInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstTinyInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUSmallInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstSmallInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUMedInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstMedInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstULongInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstLongInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstULongLongInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstLongLongInt)
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
    signed   short sbind = 8;
    unsigned short ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

