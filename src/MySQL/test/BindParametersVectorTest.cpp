
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersVectorTest, BindVectorTestAgainstChar1)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B1=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(DB::Access::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstVarChar32)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B2=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(DB::Access::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstTinyText)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B3=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(DB::Access::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstText)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B4=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(DB::Access::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstMediumText)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B5=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(DB::Access::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

TEST(BindParametersVectorTest, BindVectorTestAgainstLongText)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM BinaryTypes WHERE B6=?");
    long count = 0;
    std::vector<char> vbind       = {'K'};
    statement.execute(DB::Access::Bind(vbind),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}

