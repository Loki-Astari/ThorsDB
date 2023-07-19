
#include "gtest/gtest.h"
#include "test/TableTest.h"
#ifdef __WINNT__
#else
#include <arpa/inet.h>
#endif
#include <cstdint>

TEST(TableTypesNotCoveredTest, Expand)
{
    using namespace ThorsAnvil::DB::Access;

    auto test = []()
    {
        Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                        THOR_TESTING_MYSQL_USER,
                                        THOR_TESTING_MYSQL_PASS,
                                        THOR_TESTING_MYSQL_DB,
                                        options);

        Statement      statement(connection, "SELECT jsonCol from TypesNotCovered WHERE Id=1");
    };
    ASSERT_THROW(
        test(),
        std::runtime_error
    );
}

