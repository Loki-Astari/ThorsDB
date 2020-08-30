
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

TEST(PrepareStatementBindFailureTest, TwoBindBointsThreeProvided)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT * FROM People where sex=? or age=?"); //select * from People;
    ASSERT_THROW(
        statement.execute(DB::Access::Bind("M",29,56.67), [](int id, std::string name, short age, std::string sex, double height){}),
        std::logic_error);
}
TEST(PrepareStatementBindFailureTest, TwoBindBointsOneProvided)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT * FROM People where sex=? or age=?"); //select * from People;
    ASSERT_THROW(
        statement.execute(DB::Access::Bind("M"), [](int id, std::string name, short age, std::string sex, double height){}),
        std::logic_error);
}
TEST(PrepareStatementBindFailureTest, ZeroBindBointsTwoProvided)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::Access::Statement      statement(connection, "SELECT * FROM People where sex='M' or age=29"); //select * from People;
    ASSERT_THROW(
        statement.execute(DB::Access::Bind("M",29), [](int id, std::string name, short age, std::string sex, double height){}),
        std::logic_error);
}


