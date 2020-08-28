
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit4)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit12)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit24)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstBit48)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUTinyInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstTinyInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUSmallInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstSmallInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUMedInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstMedInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstUInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstULongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstLongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstULongLongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersLongLongTest, BindLongLongTestAgainstLongLongInt)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    signed   long long sbind = 8;
    unsigned long long ubind = 14;
    statement.execute(DB::SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(DB::SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

