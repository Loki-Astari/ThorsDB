
#ifndef THORSANVIL_MYSQL_TEST_TABLETEST_H
#define THORSANVIL_MYSQL_TEST_TABLETEST_H

#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "MySQLConfig.h"
template<typename T>
void typeGoodTest(T expected, std::string const& expr)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, expr);
    statement.execute([&expected](T const& select){
        ASSERT_EQ(expected, select);
    });
}
template<typename T, typename ExceptionType>
void typeBadTest(std::string const& expr)
{
    using namespace ThorsAnvil;

    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    SQL::Statement      statement(connection, expr);
    ASSERT_THROW(
        statement.execute([](T const& select){
            ASSERT_TRUE(false); // Should not reach here.
        }),
        ExceptionType
    );
}

#endif
