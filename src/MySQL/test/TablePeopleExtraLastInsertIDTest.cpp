
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table PeopleExtra
 *
 */

class TablePeopleExtraLastInsertIDTest: public ::testing::Test
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
		}
		virtual void TearDown()
		{
		}
};

TEST_F(TablePeopleExtraLastInsertIDTest, ModTomHanks)
{
    using namespace ThorsAnvil;

    DB::SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::SQL::Statement      statement(connection, "INSERT INTO PeopleExtra(Name, Age, Sex, Height) VALUES('Tom Hanks', 89, 'M', 34.567)");
    statement.execute();
    long lastInsertID1 = statement.lastInsertID();
    ASSERT_NE(
        0,
        lastInsertID1
    );

    statement.execute();
    long lastInsertID2 = statement.lastInsertID();
    ASSERT_NE(
        0,
        lastInsertID1
    );
    ASSERT_NE(lastInsertID1, lastInsertID2);
}

