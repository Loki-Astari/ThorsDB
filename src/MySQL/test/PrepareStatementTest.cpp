
#include "PrepareStatement.h"
#include "PackageStream.h"
#include "ConectReader.h"
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "MockStream.h"
#include <algorithm>
#include <gtest/gtest.h>
#include "MySQLConfig.h"

TEST(PrepareStatementTest, Create)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People");
}
TEST(PrepareStatementTest, Execute)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    statement.execute([&count](int id, std::string name, short age, std::string sex, double height)
                        {
                            std::cout << "Got: " << id << " : " << name << " : " << age << " : " << sex << " : " << height << "\n";
                            ++count;
                        });
    ASSERT_EQ(2, count);
}
TEST(PrepareStatementTest, ExecuteToFewArguments)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex/*, double height*/) // Not using the height
                        {}),
        std::runtime_error);
}
TEST(PrepareStatementTest, ExecuteToManyArguments)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex, double height, long extra) // ask for more than are available
                        {}),
        std::runtime_error);
}
TEST(PrepareStatementTest, ExecuteThrowWhenCalledBack)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex, double height)
                                {
                                    throw std::runtime_error("Testing a throw");
                                }
                        ),
        std::runtime_error);
}

