
#include <gtest/gtest.h>
#include "test/TableTest.h"
#include "MySQLConfig.h"

extern std::map<std::string, std::string>      options;

/*
CREATE TABLE TimeStampTypes(
    ,ST1  DATE
    ,ST2  TIME
    ,ST3  TIMESTAMP
    ,ST4  DATETIME
    ,ST5  YEAR);
*/
// INSERT INTO TimeStampTypes(ID, ST1, ST2, ST3, ST4, ST5) VALUES (1, '1969-04-25', '06:15:43', '1973-04-25 06:15:43', '1969-04-25 06:15:43', '2016');

TEST(BindParametersTimeTest, BindTimeTestAgainstDate)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
    DB::Access::UnixTimeStamp  timeStamp(924998400); 

    DB::Access::Statement      statement(connection, "SELECT ID FROM TimeStampTypes WHERE ST1=?");
    long count = 0;
    statement.execute(DB::Access::Bind(timeStamp),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}
TEST(BindParametersTimeTest, BindTimeTestAgainstTime)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
    DB::Access::UnixTimeStamp  timeStamp(31558543);    // 01/Jan/1971  

    DB::Access::Statement      statement(connection, "SELECT ID FROM TimeStampTypes WHERE ST2=?");
    long count = 0;
    statement.execute(DB::Access::Bind(timeStamp),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}
TEST(BindParametersTimeTest, BindTimeTestAgainstTimeStamp)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
    DB::Access::UnixTimeStamp  timeStamp(925020943); 


    DB::Access::Statement      statement(connection, "SELECT ID FROM TimeStampTypes WHERE ST3=?");
    long count = 0;
    statement.execute(DB::Access::Bind(timeStamp),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}
TEST(BindParametersTimeTest, BindTimeTestAgainstDateTime)
{
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
    DB::Access::UnixTimeStamp  timeStamp(925020943); 


    DB::Access::Statement      statement(connection, "SELECT ID FROM TimeStampTypes WHERE ST4=?");
    long count = 0;
    statement.execute(DB::Access::Bind(timeStamp),[&count](long id){++count;});
    ASSERT_EQ(1, count);
}
TEST(BindParametersTimeTest, BindTimeTestAgainstYear)
{
    // TODO -Fix
    using namespace ThorsAnvil;
    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);
    DB::Access::UnixTimeStamp  timeStamp(915148800); // 01/Jan/1999 00:00:00


    DB::Access::Statement      statement(connection, "SELECT ID FROM TimeStampTypes WHERE ST5=?");
    long count = 0;
    statement.execute(DB::Access::Bind(timeStamp),[&count](long id){++count;});
    // ASSERT_EQ(1, count);
}
