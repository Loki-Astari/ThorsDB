
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersIntTest, BindIntTestAgainstBit4)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstBit12)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstBit24)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstBit48)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUTinyInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstTinyInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUSmallInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstSmallInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUMedInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstMedInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstULongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstLongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstULongLongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstLongLongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(DB::Access::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::Access::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

