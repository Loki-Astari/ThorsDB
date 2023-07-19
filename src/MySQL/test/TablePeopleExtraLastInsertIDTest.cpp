
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

class TablePeopleExtraLastInsertIDTest: public ::testing::Test
{
    protected:
        // Per-test-case set-up.
        // Called before the first test in this test case.
        // Can be omitted if not needed.
        static void SetUpTestCase()
        {
#ifdef __WINNT__
            WSADATA wsaData;
            WORD wVersionRequested = MAKEWORD(2, 2);
            int err = WSAStartup(wVersionRequested, &wsaData);
            if (err != 0) {
                printf("WSAStartup failed with error: %d\n", err);
                throw std::runtime_error("Failed to set up Sockets");
            }
#endif
            executeModification("DELETE FROM PeopleExtra");
        }

        // Per-test-case tear-down.
        // Called after the last test in this test case.
        // Can be omitted if not needed.
        static void TearDownTestCase()
        {
            executeModification("DELETE FROM PeopleExtra");
#ifdef  __WINNT__
            WSACleanup();
#endif
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

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, "INSERT INTO PeopleExtra(Name, Age, Sex, Height) VALUES('Tom Hanks', 89, 'M', 34.567)");
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

