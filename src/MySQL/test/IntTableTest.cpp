
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "gtest/gtest.h"
#include "MySQLConfig.h"

TEST(IntTableTest, ReadABitIntoAChar)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT B0 FROM IntTypes where Id=1");
    statement.execute([](char select){
        ASSERT_EQ(0xA, select);
    });
}
