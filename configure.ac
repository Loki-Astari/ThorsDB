
AC_INIT([ThorsDB], [0.1], [Loki.Astari+ThorsAnvil@gmail.com])
AC_PREREQ([2.65])

# Set up the build system
if [[ ! -e build/Notes ]]; then
    git submodule init
    git submodule update
fi
ln -s .build build | echo "Build Already Setup"
AC_CONFIG_MACRO_DIR([.build/autotools/m4])
AC_CONFIG_AUX_DIR([.build/autotools/build])
AX_THOR_FUNC_BUILD


AC_CONFIG_SRCDIR([src/ThorsDB/Connection.cpp])
AC_PROG_CXX
AX_THOR_PROG_COV
AC_C_BIGENDIAN([AC_DEFINE([THOR_ENDIAN_BIG], [1], [Set for Big Endian Systems])],
               [AC_DEFINE([THOR_ENDIAN_SML], [1], [Set for small Endian Systems])])


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


AX_THOR_FUNC_USE_YAML
AX_THOR_FUNC_USE_CRYPTO

AC_ARG_ENABLE(
    [travis],
    AS_HELP_STRING([--disable-travis], [This disables test that don't work on Travis.ci but don't cause a build break])
)
AS_IF(
    [test "x$enable_travis" == "xno"],
    [
        AC_SUBST(ThorsSerialize_ROOT_DIR, [])
        AC_SUBST(ThorsSerialize_ROOT_LIB, [ThorSerialize])
    ],
    [
        AX_THOR_FUNC_USE_THORS_LIB_SERIALIZE(17)
    ]
)
AX_THOR_USE_TIMEGM
AX_THOR_USE_MODTEST

LT_INIT

AX_CHECK_COMPILE_FLAG([-ansi], [AC_SUBST([CXXFLAGS], ["$CXXFLAGS -ansi"])])

AX_THOR_FUNC_LANG_FLAG(17)

AC_DEFUN([AX_THOR_CHECK_SMARTY_AVAILABLE],
    AC_CHECK_LIB(
        [smarty],
        [__ZN6snappy10UncompressEPNS_6SourceEPNS_4SinkE]
        [],
        [
AC_MSG_ERROR([
Error: Could not find libsnappy

    On mac this can be installed with:
        brew install snappy

    On ubuntu this can be installed with:
        sudo apt-get install libsnappy-dev

])
        ]
    )
)

AC_DEFUN([AX_THOR_CHECKDB_AVAILABLE],
[
    AC_ARG_WITH([Test$2Host], AS_HELP_STRING([--with-Test$2Host=<Host>], [Use an alternative $3 host for testing with Default(127.0.0.1)]))
    AC_ARG_WITH([Test$2User], AS_HELP_STRING([--with-Test$2User=<User>], [Use an alternative $3 user for testing with (test)]))
    AC_ARG_WITH([Test$2Pass], AS_HELP_STRING([--with-Test$2Pass=<Pass>], [Use an alternative $3 password for testing with (testPassword)]))
    AC_ARG_WITH([Test$2Database], AS_HELP_STRING([--with-Test$2Database=<DB>], [Use an alternative $3 database for testing with (test)]))

    $3_test_host="127.0.0.1"
    $3_test_user="test"
    $3_test_pw="testPassword"
    $3_test_db="test"

    AS_IF([test "x$have_Test$2Host" = "xyes"], [$3_test_host=$with_Test$2Host])
    AS_IF([test "x$have_Test$2User" = "xyes"], [$3_test_user=$with_Test$2User])
    AS_IF([test "x$have_Test$2Pass" = "xyes"], [$3_test_pw=$with_Test$2Pass])
    AS_IF([test "x$have_Test$2Database" = "xyes"], [$3_test_db=$with_Test$2Database])

    AC_DEFINE_UNQUOTED([THOR_TESTING_$1_HOST], ["$$3_test_host"], [$3 DB host for testing])
    AC_DEFINE_UNQUOTED([THOR_TESTING_$1_USER], ["$$3_test_user"], [$3 DB user for testing])
    AC_DEFINE_UNQUOTED([THOR_TESTING_$1_PASS], ["$$3_test_pw"],   [$3 DB password for testing])
    AC_DEFINE_UNQUOTED([THOR_TESTING_$1_DB],   ["$$3_test_db"],   [$3 DB for testing])

    mysq_test_connect=`echo "$6" | $3 -$5 $$3_test_host -u $$3_test_user -p$$3_test_pw $$3_test_db 2> /dev/null | tail -$8 | head -1`
    AS_IF([test "x$mysq_test_connect" != "$7"],
         [AC_MSG_ERROR([

    Error: Can not connect to $3 server for testing.

            This may be because the $3 server is not running or the test data has not been created.

            1: Install $2 server
            2: Make sure $2 is running
            3: Install the test data and users.
                    cat ./src/$2/test/data/init.$4 | $3 -$5 $$3_test_host -u root -p 
                    cat ./src/$2/test/data/data.$4 | $3 -$5 $$3_test_host -u $$3_test_user -p$$3_test_pw $$3_test_db

                      ])
         ])

    version=`$3 -$5 $$3_test_host -u $$3_test_user -p$$3_test_pw $9 $$3_test_db | tail -$10 | awk '{print $$11}' | awk -F\. '{print $$12}'`
    AC_DEFINE_UNQUOTED([$1_MAJOR_VERSION], [${version}], ["Get $3 version into #define. That way we can turn off some tests"])
])
AC_DEFUN([AX_THOR_CHECKDB_AVAILABLE_MYSQL],
        [AX_THOR_CHECKDB_AVAILABLE([MYSQL], [MySQL], [mysql], [sql],
                                    [B -h],
                                    [select 3+4 from dual], [x7],
                                    [1],
                                    [-e 'SHOW VARIABLES LIKE "innodb_version"'],
                                    [1], [2], [1])
        ]
)
AC_DEFUN([AX_THOR_CHECKDB_AVAILABLE_MONGO],
        [AX_THOR_CHECKDB_AVAILABLE([MONGO], [Mongo], [mongo], [mongo],
                                    [-host],
                                    [db.Blob.stats().nindexes], [x1],
                                    [2],
                                    [--eval 'db.version()'],
                                    [1], [1], [1])
        ]
)

AX_THOR_CHECKDB_AVAILABLE_MYSQL
AX_THOR_CHECKDB_AVAILABLE_MONGO
AX_THOR_CHECK_SMARTY_AVAILABLE

export cwd=`pwd`
pushd third/ThorsIOUtil
./configure --prefix=${prefix} --with-hostbuild=${cwd}/build ${subconfigvera}
popd
pushd third/ThorsCrypto
addCrypoRoot=""
AS_IF([test "$with_cryptoroot" != ""],
      [addCrypoRoot="--with-cryptoroot=$with_cryptoroot"])
./configure --prefix=${prefix} --with-hostbuild=${cwd}/build ${subconfigvera} ${addCrypoRoot}
popd
pushd third/ThorsSocket
./configure --prefix=${prefix} --with-hostbuild=${cwd}/build ${subconfigvera} ${addCrypoRoot}
popd


AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([config.h src/MySQL/MySQLConfig.h:config.h.in src/ThorsDB/ThorsDBConfig.h:config.h.in src/ThorsDBCommon/ThorsDBConfig.h:config.h.in src/Postgres/PostgresConfig.h:config.h.in src/ThorsDBBuild/ThorsDBConfig.h:config.h.in src/ThorsDBBuildTest/ThorsDBConfig.h:config.h.in src/Mongo/MongoConfig.h:config.h.in])
AC_CONFIG_FILES([Makefile.extra Makefile.config:build/autotools/build/Makefile.config.in:Makefile.config.in])
AC_OUTPUT
