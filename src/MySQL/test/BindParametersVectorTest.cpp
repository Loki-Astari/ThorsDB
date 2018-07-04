
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersVectorTest, BindVectorTestAgainstChar1)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B1=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(SQL::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstVarChar32)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B2=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(SQL::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstTinyText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B3=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(SQL::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B4=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(SQL::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstMediumText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B5=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(SQL::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstLongText)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B6=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(SQL::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

