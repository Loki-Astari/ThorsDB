
#include "PrepareStatement.h"
#include "ThorsDBCommon/StreamInterface.h"
#include "ConectReader.h"
#include "ThorsDB/Connection.h"
#include "ThorsDB/Statement.h"
#include "MockStream.h"
#include <algorithm>
#include <gtest/gtest.h>
#include "MySQLConfig.h"
#include "test/TableTest.h"

TEST(PrepareStatementLambdaFailureTest, ExecuteToFewArguments)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex/*, double height*/) // Not using the height
                        {}),
        std::logic_error);
}
TEST(PrepareStatementLambdaFailureTest, ExecuteToManyArguments)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex, double height, long extra) // ask for more than are available
                        {}),
        std::logic_error);
}
TEST(PrepareStatementLambdaFailureTest, ExecuteThrowWhenCalledBack)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT * FROM People"); //select * from People;
    long                count = 0;
    ASSERT_THROW(
        statement.execute([&count](int id, std::string name, short age, std::string sex, double height)
                                {
                                    throw std::runtime_error("Testing a throw");
                                }
                        ),
        std::runtime_error);
}

