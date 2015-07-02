
#include "PrepareStatement.h"
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include <gtest/gtest.h>
#include "MySQLConfig.h"

TEST(PrepareStatementTest, Create)
{
#if 0
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People");
#endif
}
TEST(PrepareStatementTest, Execute)
{
#if 0
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
#endif
}

