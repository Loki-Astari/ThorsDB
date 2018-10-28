
#include "gtest/gtest.h"
#include "test/TableTest.h"
#include <arpa/inet.h>
#include <cstdint>

#if 0
MIY TODO
Breaks on Travis but not mac
TEST(TableTypesNotCoveredTest, Expand)
{
    using namespace ThorsAnvil;

    auto test = []()
    {
        SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                        THOR_TESTING_MYSQL_USER,
                                        THOR_TESTING_MYSQL_PASS,
                                        THOR_TESTING_MYSQL_DB,
                                        options);

        SQL::Statement      statement(connection, "SELECT jsonCol from TypesNotCovered WHERE Id=1");
    };
    ASSERT_THROW(
        test(),
        std::domain_error
    );
}
#endif

