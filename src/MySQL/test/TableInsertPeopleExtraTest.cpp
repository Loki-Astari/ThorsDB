
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table PeopleExtra
 *
 */

class InsertPeopleTest: public ::testing::Test
{
	protected:
		// Per-test-case set-up.
		// Called before the first test in this test case.
		// Can be omitted if not needed.
		static void SetUpTestCase()
		{
		}

		// Per-test-case tear-down.
		// Called after the last test in this test case.
		// Can be omitted if not needed.
		static void TearDownTestCase()
		{
            checkSelectCount("SELECT * FROM PeopleExtra WHERE ID=15", 0);
		}

		// You can define per-test set-up and tear-down logic as usual.
		virtual void SetUp()
		{
		}
		virtual void TearDown()
		{
            checkSelectCount("SELECT * FROM PeopleExtra WHERE ID=15", 1);
            executeModification("DELETE FROM PeopleExtra WHERE ID=15");
            checkSelectCount("SELECT * FROM PeopleExtra WHERE ID=15", 0);
		}
};

TEST_F(InsertPeopleTest, InsertTomHanks)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "INSERT INTO PeopleExtra (ID, Name, Age, Sex, Height) VALUES (15, 'Tom Hanks', 67, 'M', 5.67)");
    statement.execute();
}

TEST_F(InsertPeopleTest, InsertTomHanks2)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "INSERT INTO PeopleExtra (ID, Name, Age, Sex, Height) VALUES (?, 'Tom Hanks', 67, 'M', 5.67)");
    statement.execute(SQL::Bind(15));
}

