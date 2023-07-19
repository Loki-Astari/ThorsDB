#include "test/pipe.h"
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

std::map<std::string, std::string>      options {{"default-auth", "mysql_native_password"}};

TEST(BindParametersCharTest, BindCharTestAgainstBit4)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstBit12)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstBit24)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstBit48)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUTinyInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstTinyInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUSmallInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstSmallInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUMedInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstMedInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstUInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstULongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstULongLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstLongLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    statement.execute(DB::Access::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstChar)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S1=?");
    long count = 0;
    statement.execute(DB::Access::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstVarChar)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S2=?");
    long count = 0;
    statement.execute(DB::Access::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstTinyText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S3=?");
    long count = 0;
    statement.execute(DB::Access::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S4=?");
    long count = 0;
    statement.execute(DB::Access::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstMedText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S5=?");
    long count = 0;
    statement.execute(DB::Access::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindCharTestAgainstLongText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S6=?");
    long count = 0;
    statement.execute(DB::Access::Bind('F'),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstBit4)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstBit12)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstBit24)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstBit48)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstUTinyInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstTinyInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstUSmallInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstSmallInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstUMedInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstMedInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstUInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstULongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstULongLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstLongLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstChar)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S1=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstVarChar)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstTinyText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S4=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstMedText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S5=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindUSCharTestAgainstLongText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S6=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<unsigned char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstBit4)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstBit12)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B1=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstBit24)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long v1, unsigned long v2){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstBit48)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstUTinyInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstTinyInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstUSmallInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstSmallInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstUMedInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U4=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstMedInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I4=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstUInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U5=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I5=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstULongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U6=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I6=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstULongLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE U7=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstLongLongInt)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE I7=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('8')),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstChar)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S1=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstVarChar)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstTinyText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S4=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstMedText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S5=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

TEST(BindParametersCharTest, BindSICharTestAgainstLongText)
{
    SocketSetUp         setupSocket;

    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT ID, S1, S2 FROM StringTypes WHERE S6=?");
    long count = 0;
    statement.execute(DB::Access::Bind(static_cast<signed char>('F')),[&count](long id, std::string const& , std::string const&){if (id==2){++count;}});
    ASSERT_EQ(1, count);
}

