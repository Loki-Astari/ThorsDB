
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersLongTest, BindLongTestAgainstBit4)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstBit12)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstBit24)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstBit48)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstUTinyInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstTinyInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstUSmallInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstSmallInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstUMedInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstMedInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstUInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstULongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstLongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstULongLongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongTest, BindLongTestAgainstLongLongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    signed   long sbind = 8;
    unsigned long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

