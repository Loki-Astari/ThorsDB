
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersStringTest, BindStringTestAgainstChar1)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S1=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::SQL::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstVarChar32)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S2=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::SQL::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstTinyText)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S3=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::SQL::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstText)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S4=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::SQL::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstMediumText)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S5=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::SQL::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstLongText)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S6=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::SQL::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::SQL::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

