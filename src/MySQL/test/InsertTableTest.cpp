
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table PeopleExtra
 *
 */


// Bit Values can only be read into "unsigned integer types"a
TEST(InsertPeopleTest, InsertTomHanks)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    // CREATE TABLE PeopleExtra ( ID  INTEGER, Name VARCHAR(64), Age  SMALLINT, Sex  CHAR(1), Height DOUBLE);
    SQL::Statement      statement(connection, "INSERT INTO PeopleExtra (Name, Age, Sex, Height) VALUES ('Tom Hanks', 67, 'M', 5.67)");
    statement.execute();
}

TEST(InsertPeopleTest, UpdateTomHanks)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "UPDATE PeopleExtra SET Age=Age+1 WHERE ID=3");
    statement.execute();
}

TEST(InsertPeopleTest, DeleteTomHanks)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "DELETE FROM PeopleExtra WHERE isNull(ID)");
    statement.execute();
}


