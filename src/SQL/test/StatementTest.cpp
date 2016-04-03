
#include "Statement.h"
#include "Connection.h"
#include "SQLConfig.h"
#include <iostream>
#include <gtest/gtest.h>

TEST(StatementTest, call)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
    Statement       statement(connection, "Plop");

    statement.execute(Bind(15), [](int id, std::string const& name, short age, char sex, double height)
                                {
                                    std::cout << "Worked " << id << " : " << name << ": " << age << " : " << height << "\n";
                                }
    );
}

