#include "test/pipe.h"
#include "gtest/gtest.h"
#include "test/TableTest.h"
#ifdef __WINNT__
#else
#include <arpa/inet.h>
#endif
#include <cstdint>

/*
 * see test/data/init.sql
 * Add test data to this file in table PeopleExtra
 *
 */


class TablePeopleExtraDeleteTest: public ::testing::Test
{
    protected:
        // Per-test-case set-up.
        // Called before the first test in this test case.
        // Can be omitted if not needed.
        static void SetUpTestCase()
        {
            std::string cmd = ThorsAnvil::Utility::buildStringFromParts("bash -c './test/script/deletePeopleExtra ", THOR_TESTING_MYSQL_USER," ",  THOR_TESTING_MYSQL_PASS," ",  THOR_TESTING_MYSQL_DB, "'");
            EXPECT_EQ(0, system(cmd.c_str()));
        }

        // Per-test-case tear-down.
        // Called after the last test in this test case.
        // Can be omitted if not needed.
        static void TearDownTestCase()
        {
            std::string cmd = ThorsAnvil::Utility::buildStringFromParts("bash -c './test/script/deletePeopleExtra ", THOR_TESTING_MYSQL_USER," ",  THOR_TESTING_MYSQL_PASS," ",  THOR_TESTING_MYSQL_DB, "'");
            EXPECT_EQ(0, system(cmd.c_str()));
        }

        // You can define per-test set-up and tear-down logic as usual.
        virtual void SetUp()
        {
            std::string cmd = ThorsAnvil::Utility::buildStringFromParts("bash -c './test/script/deleteTestSetUp ", THOR_TESTING_MYSQL_USER," ",  THOR_TESTING_MYSQL_PASS," ",  THOR_TESTING_MYSQL_DB, "'");
            EXPECT_EQ(0, system(cmd.c_str()));
        }
        virtual void TearDown()
        {
            // The test should delete it. Therefore the count will be 0.
            std::string cmd = ThorsAnvil::Utility::buildStringFromParts("bash -c './test/script/deleteTestTearDown ", THOR_TESTING_MYSQL_USER," ",  THOR_TESTING_MYSQL_PASS," ",  THOR_TESTING_MYSQL_DB, "'");
            EXPECT_EQ(0, system(cmd.c_str()));
        }
};

TEST_F(TablePeopleExtraDeleteTest, DeleteTomHanks)
{
    SocketSetUp  setupSocket;

    using namespace ThorsAnvil;

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, "DELETE FROM PeopleExtra WHERE ID = 18");
    statement.execute();
}

TEST_F(TablePeopleExtraDeleteTest, DeleteTomHanksWithBind)
{
    SocketSetUp  setupSocket;

    using namespace ThorsAnvil;

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, "DELETE FROM PeopleExtra WHERE ID = ?");
    statement.execute(DB::Access::Bind(18));
}

