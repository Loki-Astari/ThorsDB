
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include "ThorSQL/SQLUtil.h"
#include <arpa/inet.h>
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table PeopleExtra
 *
 */

class TableUpdatePeopleExtraTest: public ::testing::Test
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
		}

		// You can define per-test set-up and tear-down logic as usual.
		virtual void SetUp()
		{
		}
		virtual void TearDown()
		{
		}
};

TEST_F(TableUpdatePeopleExtraTest, UpdateTomHanks)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "UPDATE PeopleExtra SET Age=Age+1 WHERE ID = 21");
    statement.execute();
}

TEST_F(TableUpdatePeopleExtraTest, UpdateTomHanksWithBind)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "UPDATE PeopleExtra SET Age=Age+1 WHERE ID = ?");
    statement.execute(SQL::Bind(21));
}

