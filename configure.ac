AC_INIT([ThorsDB], [3.0], [Loki.Astari+ThorsDB@gmail.com])
AC_PREREQ([2.65])

AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])



AX_THOR_FUNC_INIT_BUILD([ThorsDB], [src/ThorsDB/Connection.cpp])

AC_C_BIGENDIAN([AC_DEFINE([THOR_ENDIAN_BIG], [1], [Set for Big Endian Systems])],
               [AC_DEFINE([THOR_ENDIAN_SML], [1], [Set for small Endian Systems])])


AX_BOOST_BASE
AX_THOR_CHECK_USE_YAML
AX_THOR_CHECK_USE_SNAPPY
AX_THOR_CHECK_USE_CRYPTO
AX_THOR_CHECK_USE_MAGIC_ENUM
AX_THOR_CHECK_USE_THORS_SERIALIZE_HEADER_ONLY
AX_THOR_CHECK_USE_THORS_SERIALIZE

LT_INIT

AX_THOR_SERVICE_AVAILABLE_MYSQL
AX_THOR_SERVICE_AVAILABLE_POSTGRESS
AX_THOR_SERVICE_AVAILABLE_MONGO
AX_THOR_CHECK_USE_SMARTY

AX_THOR_DISABLE_TEST_REQUIREING_POSTGRESS_AUTHENTICATION
AX_THOR_DISABLE_TEST_REQUIREING_MONGO_QUERY

AX_THOR_FUNC_TERM_BUILD([THORSDB], [src/MySQL/MySQLConfig.h:localconfig.h.in src/ThorsDB/ThorsDBConfig.h:localconfig.h.in src/ThorsDBCommon/ThorsDBConfig.h:localconfig.h.in src/Postgres/PostgresConfig.h:localconfig.h.in src/ThorsDBBuild/ThorsDBConfig.h:localconfig.h.in src/ThorsDBBuildTest/ThorsDBConfig.h:localconfig.h.in src/Mongo/MongoConfig.h:localconfig.h.in])

AC_OUTPUT
