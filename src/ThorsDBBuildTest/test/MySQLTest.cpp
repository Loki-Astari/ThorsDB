
#include <gtest/gtest.h>
#include "ThorsDBBuildTestConfig.h"
#include "ThorsDB/SQLUtil.h"
#include "ThorsDB/Connection.h"
#include "ThorsDB/Statement.h"

std::map<std::string, std::string>      options {{"default-auth", "mysql_native_password"}};

TEST(BindParametersCharTest, BindCharTestAgainstBit4)
{
    using namespace ThorsAnvil;
    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    DB::SQL::Statement      statement(connection, "SELECT ID, B0,B1 FROM IntTypes WHERE B0=?");
    long count = 0;
    statement.execute(DB::SQL::Bind('8'),[&count](long id, unsigned long, unsigned long){if (id==4){++count;}});
    ASSERT_EQ(1, count);
}

