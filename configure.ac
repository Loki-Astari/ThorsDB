
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


AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_HEADERS([src/MySQL/MySQLConfig.h])
AC_CONFIG_FILES([Makefile.extra Makefile.config])
AC_OUTPUT
