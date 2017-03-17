
#ifndef THORS_ANVIL_MYSQL_TEST_TABLETEST_H
#define THORS_ANVIL_MYSQL_TEST_TABLETEST_H

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

inline std::string getMySQL()
{
    return ThorsAnvil::stringBuild("mysql --user=", THOR_TESTING_MYSQL_USER,
                                        " --password=", THOR_TESTING_MYSQL_PASS,
                                        " ", THOR_TESTING_MYSQL_DB,
                                        " 2> /dev/null");
}

inline std::string getSelectCount(std::string const& select)
{
    return ThorsAnvil::stringBuild("echo '", select, "'",
                                    " | ",
                                    getMySQL(),
                                    " | ",
                                    " wc -l",
                                    " | ",
                                    // Remove the Header row
                                    " awk '{if ($1 == 0){print 0;}else{print $1-1;}}'");
}

inline void checkSelectCount(std::string const& select, int row)
{
    std::string checkNoRowsLeft = ThorsAnvil::stringBuild(
        "if ( test $(", getSelectCount(select), ") != ", row, " ); then exit 1; fi"
                                                         );
    ASSERT_EQ(
        0,
        system(checkNoRowsLeft.c_str())
    );
}

static void executeModification(std::string const& mod)
{
    std::string modString = ThorsAnvil::stringBuild("echo '", mod , "' | ", getMySQL());
    ASSERT_EQ(
        0,
        system(modString.c_str())
    );
}


#endif
