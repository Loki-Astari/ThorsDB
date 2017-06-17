
AC_INIT([ThorsSQL], [0.1], [Loki.Astari+ThorsAnvil@gmail.com])
AC_PREREQ([2.65])

# Set up the build system
AC_CONFIG_MACRO_DIR([build/autotools/m4])
AX_THOR_FUNC_BUILD
AC_CONFIG_AUX_DIR([build/autotools/build])


AC_CONFIG_SRCDIR([src/SQL/Connection.cpp])
AC_PROG_CXX
AX_THOR_PROG_COV
AC_C_BIGENDIAN([AC_DEFINE([THOR_ENDIAN_BIG], [1], [Set for Big Endian Systems])],
               [AC_DEFINE([THOR_ENDIAN_SML], [1], [Set for small Endian Systems])])


AC_DEFUN([AX_THOR_FUNC_USE_CRYPTO],
[
    AC_ARG_WITH(
        [cryptoroot],
        AS_HELP_STRING([--with-cryptoroot=<location>], [Directory of CRYPTO_ROOT])
    )
    ORIG_LDFLAGS="${LDFLAGS}"
    LDFLAGS="$LDFLAGS -L$with_cryptoroot/lib"

    AC_CHECK_LIB(
        [crypto],
        [SHA1_Init],
        [
            AS_IF([test "$with_cryptoroot" != ""],
                  [
                    AC_SUBST([crypto_ROOT_LIBDIR], ["-L $with_cryptoroot/lib"])
                    AC_SUBST([crypto_ROOT_INCDIR], ["-I $with_cryptoroot/include"])
                  ])
        ],
        [AC_MSG_ERROR([
 
Error: Could not find libcrypto

        On a mac you will need to install openssl
        and define the crypto root directory to configuration.

            brew install openssl
            ./configure --with-cryptoroot=/usr/local/Cellar/openssl/1.0.2j/

        On Linux you will need to install openssl

            sudo apt-get install openssl

                ], [1])]
    )

    LDFLAGS="${ORIG_LDFLAGS}"
])

AC_DEFUN([AX_THOR_USE_TIMEGM],
[
    AC_ARG_WITH(
        [timegm],
        [AS_HELP_STRING([--without-timegm], [Disables tests that use the timegm functions])]
    )

    AS_IF([test "x$with_timegm" != xno],
    [
        AC_CHECK_FUNCS(
            [timegm],
            [],
            [
                AC_MSG_ERROR([

Error: Could not find `timegm()` function on your system.

    timegm() is only used for testing.
    If you are just building this package for usage (and trust that test will work) then you can safely disable this test.
    If you are using modifying this package you will need to fix this because you need to run all the tests before submitting a pull request.

    To disable these test by specifying --without-timegm

    PS: If you happen to know a valid alternative to timegm() that is POSIX standard I would be greatful for input.
                ])
            ]
        )
    ],
    [
        AC_DEFINE([THOR_USE_TIMEGM_FLASE], [1], [Disable tests that use timegm()])
    ])
])

AC_DEFUN([AX_THOR_USE_MODTEST],
[
    AC_ARG_WITH(
        [modtests],
        [AS_HELP_STRING([--without-modtests], [Disables tests that check that modifying the DB work])]
    )

    AS_IF([test "x$with_modtests" != xno],
    [
        thor_clt=good
        AC_CHECK_TOOL([echo], [], [thor_clt=bad])
        AC_CHECK_TOOL([wc],   [], [thor_clt=bad])
        AC_CHECK_TOOL([awk],  [], [thor_clt=bad])
        AS_IF([test "x$thor_clt" == xbad],
        [
            AC_MSG_ERROR([

Error: Could not find one or more of: echo wc awk

    echo/awk/ws are only used for testing.
    If you are just building this package for usage (and trust that test will work) then you can safely disable this test.
    If you are using modifying this package you will need to fix this because you need to run all the tests before submitting a pull request.

    To disable these test by specifying --without-modtests

            ])
        ])
    ],
    [
        AC_DEFINE([THOR_USE_MOD_TESTS_FLASE], [1], [Disable tests that use timegm()])
    ])
])


AX_THOR_FUNC_USE_CRYPTO
AX_THOR_USE_TIMEGM
AX_THOR_USE_MODTEST

LT_INIT

AX_CHECK_COMPILE_FLAG([-ansi], [AC_SUBST([CXXFLAGS], ["$CXXFLAGS -ansi"])])

AX_THOR_FUNC_LANG_FLAG(14)

AC_ARG_WITH([TestMySQLHost], AS_HELP_STRING([--with-TestMySQLHost=<Host>], [Use an alternative mysql host for testing with Default(127.0.0.1)]))
AC_ARG_WITH([TestMySQLUser], AS_HELP_STRING([--with-TestMySQLUser=<User>], [Use an alternative mysql user for testing with (test)]))
AC_ARG_WITH([TestMySQLPass], AS_HELP_STRING([--with-TestMySQLPass=<Pass>], [Use an alternative mysql password for testing with (testPassword)]))
AC_ARG_WITH([TestMySQLDatabase], AS_HELP_STRING([--with-TestMySQLDatabase=<DB>], [Use an alternative mysql database for testing with (test)]))

mysql_test_host="127.0.0.1"
mysql_test_user="test"
mysql_test_pw="testPassword"
mysql_test_db="test"

AS_IF([test "x$have_TestMySQLHost" = "xyes"], [mysql_test_host=$with_TestMySQLHost])
AS_IF([test "x$have_TestMySQLUser" = "xyes"], [mysql_test_user=$with_TestMySQLUser])
AS_IF([test "x$have_TestMySQLPass" = "xyes"], [mysql_test_pw=$with_TestMySQLPass])
AS_IF([test "x$have_TestMySQLDatabase" = "xyes"], [mysql_test_db=$with_TestMySQLDatabase])

AC_DEFINE_UNQUOTED([THOR_TESTING_MYSQL_HOST], ["$mysql_test_host"], [mysql DB host for testing])
AC_DEFINE_UNQUOTED([THOR_TESTING_MYSQL_USER], ["$mysql_test_user"], [mysql DB user for testing])
AC_DEFINE_UNQUOTED([THOR_TESTING_MYSQL_PASS], ["$mysql_test_pw"],   [mysql DB password for testing])
AC_DEFINE_UNQUOTED([THOR_TESTING_MYSQL_DB],   ["$mysql_test_db"],   [mysql DB for testing])

mysq_test_connect=`echo "select 3+4 from dual" |  mysql -h $mysql_test_host -u $mysql_test_user -p$mysql_test_pw $mysql_test_db 2> /dev/null | tail -1`
AS_IF([test "x$mysq_test_connect" != "x7"],
     [AC_MSG_ERROR([

Error: Can not connect to mysql server for testing.

        This may be because the mysql server is not running or the test data has not been created.

        1) Install MySQL server
        2) Make sure MySQL is running
        3) Install the test data and users.
                cat ./src/MySQL/test/data/init.sql | mysql -u root -p

                  ])
     ])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_HEADERS([src/MySQL/MySQLConfig.h src/SQL/SQLConfig.h])
AC_CONFIG_FILES([Makefile.extra Makefile.config:build/autotools/build/Makefile.config.in:Makefile.config.in])
AC_OUTPUT
