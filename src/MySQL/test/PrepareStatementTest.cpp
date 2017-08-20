
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
TEST(PrepareStatementTest, ExecuteWithBind)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People where sex=?"); //select * from People;
    long                count = 0;
    statement.execute(SQL::Bind("M"), [&count](int id, std::string name, short age, std::string sex, double height)
                        {
                            std::cout << "Got: " << id << " : " << name << " : " << age << " : " << sex << " : " << height << "\n";
                            ++count;
                        });
    ASSERT_EQ(1, count);
}
TEST(PrepareStatementTest, ExecuteWithBindTwo)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);


    SQL::Statement      statement(connection, "SELECT * FROM People where sex=? or age=?"); //select * from People;
    long                count = 0;
    statement.execute(SQL::Bind("M",29), [&count](int id, std::string name, short age, std::string sex, double height)
                        {
                            std::cout << "Got: " << id << " : " << name << " : " << age << " : " << sex << " : " << height << "\n";
                            ++count;
                        });
    ASSERT_EQ(2, count);
}
TEST(PrepareStatementTest, DestructorTest)
{
    using namespace ThorsAnvil;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB);
    int count = 0;
    {
        SQL::Statement      statement(connection, "SELECT * FROM People where sex=? or age=?"); //select * from People;
        statement.execute(SQL::Bind("M",29), [&count](int id, std::string name, short age, std::string sex, double height)
                            {
                                std::cout << "Got: " << id << " : " << name << " : " << age << " : " << sex << " : " << height << "\n";
                                ++count;
                            });
        ASSERT_EQ(count, 2);
    }
    {
        SQL::Statement      statement(connection, "SELECT * FROM People where sex=? or age=?"); //select * from People;
        statement.execute(SQL::Bind("M",29), [&count](int id, std::string name, short age, std::string sex, double height)
                            {
                                std::cout << "Got: " << id << " : " << name << " : " << age << " : " << sex << " : " << height << "\n";
                                ++count;
                            });
        ASSERT_EQ(count, 4);
    }
}


