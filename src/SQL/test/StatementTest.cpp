
#include "Statement.h"
#include "Connection.h"
#include "SQLConfig.h"
#include "test/SetGoodToTrue.h"
#include <iostream>
#include <gtest/gtest.h>

TEST(StatementTest, SelectWithParamOK)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");
    bool            good = false;

    statement.execute(Bind(15), SetGoodToTrue(good));
    ASSERT_TRUE(good);
}

TEST(StatementTest, SelectNoParamOK)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");
    bool            good = false;

    statement.execute(SetGoodToTrue(good));
    ASSERT_TRUE(good);
}

TEST(StatementTest, SelectWithParamFail)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    ASSERT_THROW(
        statement.execute(Bind(15)),
        std::logic_error
    );
}

TEST(StatementTest, SelectNoParamFail)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    ASSERT_THROW(
        statement.execute(),
        std::logic_error
    );
}

TEST(StatementTest, InsertParamFail)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");
    bool            good = false;

    ASSERT_THROW(
        statement.execute(Bind(15), SetGoodToTrue(good)),
        std::logic_error);
}

TEST(StatementTest, InsertNoParamFail)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");
    bool            good = false;

    ASSERT_THROW(
        statement.execute(SetGoodToTrue(good)),
        std::logic_error);
}

TEST(StatementTest, InsertParamOK)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    ASSERT_NO_THROW(
        statement.execute(Bind(15));
    );
}

TEST(StatementTest, InsertNoParamOK)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    ASSERT_NO_THROW(
        statement.execute(Bind(15));
    );
}

TEST(StatementTest, SelectGetRowsAffected)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");
    bool            good = false;

    statement.execute(Bind(15), SetGoodToTrue(good));
    ASSERT_THROW(
        statement.rowsAffected(),
        std::logic_error
    );
}

TEST(StatementTest, SelectGetLastInsertID)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");
    bool            good = false;

    statement.execute(Bind(15), SetGoodToTrue(good));
    ASSERT_THROW(
        statement.lastInsertID(),
        std::logic_error
    );
}


TEST(StatementTest, InsertGetRowsAffected)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    statement.execute(Bind(15));
    ASSERT_NO_THROW(
        statement.rowsAffected();
    );
}

TEST(StatementTest, InsertGetLastInsertID)
{
    using ThorsAnvil::DB::SQL::Connection;
    using ThorsAnvil::DB::SQL::Statement;
    using ThorsAnvil::DB::SQL::Bind;

    Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    statement.execute(Bind(15));
    ASSERT_NO_THROW(
        statement.lastInsertID();
    );
}
TEST(StatementTest, ProxyAbort)
{
    auto test = []()
    {
        using ThorsAnvil::DB::SQL::Connection;
        using ThorsAnvil::DB::SQL::Statement;
        using ThorsAnvil::DB::SQL::Bind;

        Connection      connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
        Statement       statement(connection, "SELECT 3");
        int             count = 0;

        statement.execute(Bind(15), CountLines(count));
    };
    EXPECT_THROW(
        test(),
        std::domain_error
    );
}

TEST(StatementTest, UnixTimeStampDefaultInit)
{
    ThorsAnvil::DB::SQL::UnixTimeStamp      test;
    EXPECT_EQ(0, test.time);
}
TEST(StatementTest, UnixTimeStampInit)
{
    ThorsAnvil::DB::SQL::UnixTimeStamp      test(15);
    EXPECT_EQ(15, test.time);
}
TEST(StatementTest, UnixTimeStampEquiv)
{
    ThorsAnvil::DB::SQL::UnixTimeStamp      lhs(15);
    ThorsAnvil::DB::SQL::UnixTimeStamp      rhs(15);

    EXPECT_TRUE(lhs == rhs);
}
TEST(StatementTest, UnixTimeStampNonEquiv)
{
    ThorsAnvil::DB::SQL::UnixTimeStamp      lhs;
    ThorsAnvil::DB::SQL::UnixTimeStamp      rhs(15);

    EXPECT_TRUE(lhs != rhs);
}
TEST(StatementTest, ValidationTmpErrorBuild)
{
    ThorsAnvil::DB::SQL::Lib::ValidationTmpError    error("Test");
}

