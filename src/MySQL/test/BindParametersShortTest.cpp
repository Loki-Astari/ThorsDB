
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersShortTest, BindShortTestAgainstBit4)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstBit12)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstBit24)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstBit48)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUTinyInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstTinyInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUSmallInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstSmallInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUMedInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstMedInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstUInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstULongInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstLongInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstULongLongInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

TEST(BindParametersShortTest, BindShortTestAgainstLongLongInt)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    statement.execute(SQL::Bind(8),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    statement.execute(SQL::Bind(264),[&count](long id, unsigned long, unsigned long){if (id==5){++count;}});
    ASSERT_EQ(2, count);
}

