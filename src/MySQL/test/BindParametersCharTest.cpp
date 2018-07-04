
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

std::map<std::string, std::string>      options {{"default-auth", "mysql_native_password"}};

TEST(BindParametersCharTest, BindCharTestAgainstBit4)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstBit12)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstBit24)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstBit48)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUTinyInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstTinyInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUSmallInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstSmallInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUMedInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstMedInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstULongInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstLongInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstULongLongInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstLongLongInt)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    statement.execute(SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstChar)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S1=?");
    long count = 0;
    statement.execute(SQL::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstVarChar)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S2=?");
    long count = 0;
    statement.execute(SQL::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstTinyText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S3=?");
    long count = 0;
    statement.execute(SQL::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S4=?");
    long count = 0;
    statement.execute(SQL::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstMedText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S5=?");
    long count = 0;
    statement.execute(SQL::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstLongText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S6=?");
    long count = 0;
    statement.execute(SQL::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

