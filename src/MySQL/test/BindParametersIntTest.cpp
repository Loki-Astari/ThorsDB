
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersIntTest, BindIntTestAgainstBit4)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstBit12)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstBit24)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstBit48)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUTinyInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstTinyInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUSmallInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstSmallInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUMedInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstMedInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstUInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstULongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstLongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstULongLongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

TEST(BindParametersIntTest, BindIntTestAgainstLongLongInt)
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
    signed   int sbind = 8;
    unsigned int ubind = 14;
    statement.execute(SQL::Bind(sbind),[&count](long id, unsigned long, unsigned long){++count;});
    statement.execute(SQL::Bind(ubind),[&count](long id, unsigned long, unsigned long){++count;});
    ASSERT_EQ(2, count);
}

