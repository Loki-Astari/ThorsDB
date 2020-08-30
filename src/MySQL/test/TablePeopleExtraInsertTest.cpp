
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

class TablePeopleExtraInsertTest: public ::testing::Test
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
            checkSelectCount("SELECT * FROM PeopleExtra WHERE ID=15", 0);
		}
		virtual void TearDown()
		{
            checkSelectCount("SELECT * FROM PeopleExtra WHERE ID=15", 1);
            executeModification("DELETE FROM PeopleExtra");
		}
};

TEST_F(TablePeopleExtraInsertTest, InsertTomHanks)
{
    using namespace ThorsAnvil;

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, "INSERT INTO PeopleExtra (ID, Name, Age, Sex, Height) VALUES (15, 'Tom Hanks', 67, 'M', 5.67)");
    statement.execute();
}

TEST_F(TablePeopleExtraInsertTest, InsertTomHanksWithBind)
{
    using namespace ThorsAnvil;

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, "INSERT INTO PeopleExtra (ID, Name, Age, Sex, Height) VALUES (?, 'Tom Hanks', 67, 'M', 5.67)");
    statement.execute(DB::Access::Bind(15));
}

#endif

