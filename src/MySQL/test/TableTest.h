#ifndef THORSANVIL_MYSQL_TEST_TABLETEST_H
#define THORSANVIL_MYSQL_TEST_TABLETEST_H

#include "test/pipe.h"
#include "ThorsDB/Connection.h"
#include "ThorsDB/Statement.h"
#include "MySQLConfig.h"
#include "ThorsIOUtil/Utility.h"

// Defined in test/BindParametersCharTest.cpp
extern std::map<std::string, std::string>      options;

template<typename T>
inline void typeGoodTest(T expected, std::string const& expr)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, expr);
    statement.execute([&expected](T const& select){
        ASSERT_EQ(expected, select);
    });
}
template<>
inline void typeGoodTest(std::vector<char> expected, std::string const& expr)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, expr);
    statement.execute([&expected](std::vector<char> const& select){
        for(int loop = 0; loop < select.size(); ++loop)
        {
            ASSERT_EQ(expected[loop], select[loop]);
        }
    });
}

template<typename T, typename ExceptionType>
inline void typeBadTest(std::string const& expr)
{
    SocketSetUp     setupSockets;

    using namespace ThorsAnvil;

    DB::Access::Connection     connection("mysql://" THOR_TESTING_MYSQL_HOST,
                                    THOR_TESTING_MYSQL_USER,
                                    THOR_TESTING_MYSQL_PASS,
                                    THOR_TESTING_MYSQL_DB,
                                    options);

    DB::Access::Statement      statement(connection, expr);
    ASSERT_THROW(
        statement.execute([](T const& select){
            ASSERT_TRUE(false); // Should not reach here.
        }),
        ExceptionType
    );
}

inline std::string getMySQL()
{
    return ThorsAnvil::Utility::buildStringFromParts(
                                       "mysql --user=", THOR_TESTING_MYSQL_USER,
                                        " --password=", THOR_TESTING_MYSQL_PASS,
                                        " ", THOR_TESTING_MYSQL_DB,
#ifdef __WINNT__
                                        " 2> .ignore"
#else
                                        " 2> /dev/null"
#endif
    );
}

inline std::string getSelectCount(std::string const& select)
{
    return ThorsAnvil::Utility::buildStringFromParts(
                                    "echo ",
#ifdef  __WINNT__
                                    select,
#else
                                    "'", select, "'",
#endif
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
    std::string checkNoRowsLeft = ThorsAnvil::Utility::buildStringFromParts(
        "if ( test $(", getSelectCount(select), ") != ", row, " ); then exit 1; fi"
                                                         );
    ASSERT_EQ(
        0,
        system(checkNoRowsLeft.c_str())
    );
}

static void executeModification(std::string const& mod)
{
    std::string modString = ThorsAnvil::Utility::buildStringFromParts(
        "echo ",
#ifdef __WINNT__
        mod ,
#else
        "'", mod, "'",
#endif
        " | ", getMySQL());
    ASSERT_EQ(
        0,
        system(modString.c_str())
    );
}


#endif
