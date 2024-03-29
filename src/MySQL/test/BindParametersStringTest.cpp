
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

TEST(BindParametersStringTest, BindStringTestAgainstChar1)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S1=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::Access::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstVarChar32)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S2=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::Access::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstTinyText)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S3=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::Access::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstText)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S4=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::Access::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstMediumText)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S5=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::Access::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

TEST(BindParametersStringTest, BindStringTestAgainstLongText)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID FROM StringTypes WHERE S6=?");
    long count = 0;
    std::string sbind       = "F";
    char const  acbind[]    = "F";
    char        avbind[]     = "F";
    char const* ccbind       = "F";
    char*       cvbind       = "F";
    statement.execute(DB::Access::Bind(sbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(ccbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(cvbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(acbind),[&count](long id){++count;});
    statement.execute(DB::Access::Bind(avbind),[&count](long id){++count;});
    ASSERT_EQ(5, count);
}

