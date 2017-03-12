
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

class TablePeopleExtraRowsAffectedTest: public ::testing::Test
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
            executeModification("INSERT INTO PeopleExtra(ID, Name, Age, Sex, Height) VALUES(38, \"Tom Hanks\", 56, \"M\", 78.342)");
            executeModification("INSERT INTO PeopleExtra(ID, Name, Age, Sex, Height) VALUES(39, \"Tom Hanks\", 23, \"F\", 897.321)");
            executeModification("INSERT INTO PeopleExtra(ID, Name, Age, Sex, Height) VALUES(40, \"Tom Hanks\", 11, \"F\", 87.213)");
		}
		virtual void TearDown()
		{
            executeModification("DELETE FROM PeopleExtra");
		}
};

TEST_F(TablePeopleExtraRowsAffectedTest, ModTomHanks)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "UPDATE PeopleExtra SET Age=Age+1 WHERE ID >= 39");
    statement.execute();
    ASSERT_EQ(
        2,
        statement.rowsAffected()
    );
}

TEST_F(TablePeopleExtraRowsAffectedTest, ModTomHanksWithBind)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, "UPDATE PeopleExtra SET Age=Age+1 WHERE ID >= ?");
    statement.execute(SQL::Bind(39));
    ASSERT_EQ(
        2,
        statement.rowsAffected()
    );
}

TEST_F(TablePeopleExtraRowsAffectedTest, ModTomHanksNoHits)
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
    ASSERT_EQ(
        0,
        statement.rowsAffected()
    );
}

TEST_F(TablePeopleExtraRowsAffectedTest, ModTomHanksWithBindNoHits)
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
    ASSERT_EQ(
        0,
        statement.rowsAffected()
    );
}
