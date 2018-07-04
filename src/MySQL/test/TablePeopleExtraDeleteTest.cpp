
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table PeopleExtra
 *
 */

#ifndef THOR_USE_MOD_TESTS_FLASE

class TablePeopleExtraDeleteTest: public ::testing::Test
{
	protected:
		// Per-test-case set-up.
		// Called before the first test in this test case.
		// Can be omitted if not needed.
		static void SetUpTestCase()
		{
            executeModification("DELETE FROM PeopleExtra");
		}

		// Per-test-case tear-down.
		// Called after the last test in this test case.
		// Can be omitted if not needed.
		static void TearDownTestCase()
		{
            executeModification("DELETE FROM PeopleExtra");
		}

		// You can define per-test set-up and tear-down logic as usual.
		virtual void SetUp()
		{
            executeModification("INSERT INTO PeopleExtra(ID, Name, Age, Sex, Height) VALUES (18, \"Tom Hanks\", 35, \"M\", 56.89)");
            executeModification("INSERT INTO PeopleExtra(ID, Name, Age, Sex, Height) VALUES (19, \"Tom Hanks\", 35, \"M\", 56.89)");
            checkSelectCount("SELECT * FROM PeopleExtra", 2);
            checkSelectCount("SELECT * FROM PeopleExtra WHERE ID = 18", 1);
		}
		virtual void TearDown()
		{
            // The test should delete it. Therefore the count will be 0.
            checkSelectCount("SELECT * FROM PeopleExtra WHERE ID = 18", 0);
            checkSelectCount("SELECT * FROM PeopleExtra", 1);
            executeModification("DELETE FROM PeopleExtra");
		}
};

TEST_F(TablePeopleExtraDeleteTest, DeleteTomHanks)
{
    using namespace ThorsAnvil;

    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "DELETE FROM PeopleExtra WHERE ID = 18");
    statement.execute();
}

TEST_F(TablePeopleExtraDeleteTest, DeleteTomHanksWithBind)
{
    using namespace ThorsAnvil;

    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "DELETE FROM PeopleExtra WHERE ID = ?");
    statement.execute(SQL::Bind(18));
}

#endif

