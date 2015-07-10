
#include "PrepareStatement.h"
#include "PackageStream.h"
#include "ConectReader.h"
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "MockStream.h"
#include <algorithm>
#include <gtest/gtest.h>
#include "MySQLConfig.h"

TEST(PrepareStatementBindFailureTest, TwoBindBointsThreeProvided)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People where sex=? or age=?"); //select * from People;
    ASSERT_THROW(
        statement.execute(SQL::Bind("M",29,56.67), [](int id, std::string name, short age, std::string sex, double height){}),
        std::runtime_error);
}
TEST(PrepareStatementBindFailureTest, TwoBindBointsOneProvided)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People where sex=? or age=?"); //select * from People;
    ASSERT_THROW(
        statement.execute(SQL::Bind("M"), [](int id, std::string name, short age, std::string sex, double height){}),
        std::runtime_error);
}
TEST(PrepareStatementBindFailureTest, ZeroBindBointsTwoProvided)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People where sex='M' or age=29"); //select * from People;
    ASSERT_THROW(
        statement.execute(SQL::Bind("M",29), [](int id, std::string name, short age, std::string sex, double height){}),
        std::runtime_error);
}


