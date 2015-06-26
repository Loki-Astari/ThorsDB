
#include "Statement.h"
#include "Connection.h"
#include <iostream>
#include <gtest/gtest.h>

TEST(StatementTest, call)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Statement;
    using ThorsAnvil::SQL::Bind;

    Connection      connection("mysql://127.0.0.1:69", "root", "testPassword", "test");
    Statement       statement(connection, "Plop");

    statement.execute(Bind(15), [](int id){std::cout << "Worked " << id << "\n";});
}

