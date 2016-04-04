
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


AC_CHECK_LIB(
            [crypto],
            [SHA1_Init],
            [],
            [AC_MSG_ERROR([

The MySQL library requires SSH1 crypto to generate the password hash used in authentication.
Please install libcrypto on your machine.

            ])])
 
Error: Could not find libyaml

You can solve this by installing libyaml
    see http://pyyaml.org/wiki/LibYAML

LT_INIT

AX_CHECK_COMPILE_FLAG([-ansi], [AC_SUBST([CXXFLAGS], ["$CXXFLAGS -ansi"])])

AX_THOR_FUNC_LANG_FLAG(14)

AC_ARG_WITH([TestMySQLHost], AS_HELP_STRING([--with-TestMySQLHost=<Host>], [Use an alternative mysql host for testing with Default(127.0.0.1)]))
AC_ARG_WITH([TestMySQLUser], AS_HELP_STRING([--with-TestMySQLUser=<User>], [Use an alternative mysql user for testing with (test)]))
AC_ARG_WITH([TestMySQLPass], AS_HELP_STRING([--with-TestMySQLPass=<Pass>], [Use an alternative mysql password for testing with (testPassword)]))
AC_ARG_WITH([TestMySQLDatabase], AS_HELP_STRING([--with-TestMySQLDatabase=<DB>], [Use an alternative mysql database for testing with (test)]))

AS_IF([test "x$have_TestMySQLHost" = "xyes"],
            [AC_DEFINE([THOR_TESTING_MYSQL_HOST], [$with_TestMySQLHost],   [mysql DB host for testing])],
            [AC_DEFINE([THOR_TESTING_MYSQL_HOST], ["127.0.0.1"],           [mysql DB host for testing])]
)
AS_IF([test "x$have_TestMySQLUser" = "xyes"],
            [AC_DEFINE([THOR_TESTING_MYSQL_USER], [$with_TestMySQLUser],   [mysql DB user for testing])],
            [AC_DEFINE([THOR_TESTING_MYSQL_USER], ["test"],                [mysql DB user for testing])]
)
AS_IF([test "x$have_TestMySQLPass" = "xyes"],
            [AC_DEFINE([THOR_TESTING_MYSQL_PASS], [$with_TestMySQLPass],   [mysql DB password for testing])],
            [AC_DEFINE([THOR_TESTING_MYSQL_PASS], ["testPassword"],        [mysql DB password for testing])]
)
AS_IF([test "x$have_TestMySQLDatabase" = "xyes"],
            [AC_DEFINE([THOR_TESTING_MYSQL_DB], [$with_TestMySQLDatabase], [mysql DB password for testing])],
            [AC_DEFINE([THOR_TESTING_MYSQL_DB], ["test"],                  [mysql DB password for testing])]
)

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_HEADERS([src/MySQL/MySQLConfig.h src/SQL/SQLConfig.h])
AC_CONFIG_FILES([Makefile.extra Makefile.config])
AC_OUTPUT
