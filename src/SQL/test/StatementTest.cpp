
#include "Statement.h"
#include "Connection.h"
#include "SQLConfig.h"
#include <iostream>
#include <gtest/gtest.h>

TEST(StatementTest, SelectWithParamOK)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    bool            good = false;

    statement.execute(Bind(15), [&good](int id, std::string const& name, short age, char sex, double height)
                                {
                                    good = true;
                                }
    );
    ASSERT_TRUE(good);
}

TEST(StatementTest, SelectNoParamOK)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    bool            good = false;

    statement.execute([&good](int id, std::string const& name, short age, char sex, double height)
                      {
                          good = true;
                      }
    );
    ASSERT_TRUE(good);
}

TEST(StatementTest, SelectWithParamFail)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    ASSERT_THROW(
        statement.execute(Bind(15)),
        std::logic_error
    );
}

TEST(StatementTest, SelectNoParamFail)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    ASSERT_THROW(
        statement.execute(),
        std::logic_error
    );
}

TEST(StatementTest, InsertParamFail)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    ASSERT_THROW(
        statement.execute(Bind(15), [](int id, std::string const& name, short age, char sex, double height) {}),
        std::logic_error);
}

TEST(StatementTest, InsertNoParamFail)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    ASSERT_THROW(
        statement.execute([](int id, std::string const& name, short age, char sex, double height) {}),
        std::logic_error);
}

TEST(StatementTest, InsertParamOK)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    ASSERT_NO_THROW(
        statement.execute(Bind(15));
    );
}

TEST(StatementTest, InsertNoParamOK)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    ASSERT_NO_THROW(
        statement.execute(Bind(15));
    );
}

TEST(StatementTest, SelectGetRowsAffected)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    statement.execute(Bind(15),[](int id, std::string const& name, short age, char sex, double height){});
    ASSERT_THROW(
        statement.rowsAffected(),
        std::logic_error
    );
}

TEST(StatementTest, SelectGetLastInsertID)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "SELECT");

    statement.execute(Bind(15),[](int id, std::string const& name, short age, char sex, double height){});
    ASSERT_THROW(
        statement.lastInsertID(),
        std::logic_error
    );
}


TEST(StatementTest, InsertGetRowsAffected)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    statement.execute(Bind(15));
    ASSERT_NO_THROW(
        statement.rowsAffected();
    );
}

TEST(StatementTest, InsertGetLastInsertID)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "INSERT");

    statement.execute(Bind(15));
    ASSERT_NO_THROW(
        statement.lastInsertID();
    );
}


