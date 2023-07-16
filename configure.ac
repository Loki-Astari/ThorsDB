AC_INIT([ThorsDB], [3.0], [Loki.Astari+ThorsDB@gmail.com])
AC_PREREQ([2.65])

AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])

AX_THOR_FUNC_INIT_BUILD([ThorsDB], [17], [src/ThorsDB/Connection.cpp])

AC_C_BIGENDIAN([AC_DEFINE([THOR_ENDIAN_BIG], [1], [Set for Big Endian Systems])],
               [AC_DEFINE([THOR_ENDIAN_SML], [1], [Set for small Endian Systems])])


AX_BOOST_BASE
AX_THOR_CHECK_USE_YAML
AX_THOR_CHECK_USE_SNAPPY
AX_THOR_CHECK_USE_CRYPTO
AX_THOR_CHECK_USE_THORS_SERIALIZE

LT_INIT

AX_THOR_SERVICE_AVAILABLE_MYSQL
AX_THOR_SERVICE_AVAILABLE_MONGO
AX_THOR_CHECK_USE_SMARTY

# Before MYSQL-5 SSH was not supported for authentication
#--disable-mysql-test-server-version-below-5     X_THOR_DISABLE_MYSQL_TEST_SERVER_VERSION_BELOW_5


#
# Version up to 3.6 supported:
#   OP_REPLY (1)/ OP_UPDATE (2001)/ OP_INSERT (2002)/ OP_QUERY (2004)/ OP_GET_MORE (2005)/ OP_DELETE (2006)/ OP_KILL_CURSOR (2007)/ OP_COMPRESSED (2012)
#define THOR_DISABLE_MONGO_TEST_SERVER_VERSION_BELOW_3_6

#
# Versions from 3.6 to 4.4 supported:
#   Primary:
#       OP_REPLY (1)/ OP_UPDATE (2001)/ OP_INSERT (2002)/ OP_QUERY (2004)/ OP_GET_MORE (2005)/ OP_DELETE (2006)/ OP_KILL_CURSOR (2007)/ OP_COMPRESSED (2012)
#   Alternative:
#       OP_MSG (2013)

# Versions from 4.0 to 5.0 supported:
#   Primary:
#       OP_MSG (2013)
#       OP_COMPRESSED (2012)
#   Alternative:
#       OP_REPLY (1)/ OP_UPDATE (2001)/ OP_INSERT (2002)/ OP_QUERY (2004)/ OP_GET_MORE (2005)/ OP_DELETE (2006)/ OP_KILL_CURSOR (2007)

# Version 5.0:
#   Primary:
#       OP_MSG (2013)
#       OP_COMPRESSED (2012)
#   Deprecated:
#       OP_REPLY (1)/ OP_UPDATE (2001)/ OP_INSERT (2002)/ OP_QUERY (2004)/ OP_GET_MORE (2005)/ OP_DELETE (2006)/ OP_KILL_CURSOR (2007)/ OP_COMPRESSED (2012)

# Version 5.1 Onwards:
#   Primary:
#       OP_MSG (2013)
#       OP_COMPRESSED (2012)

#--disable-mogo-test-server-op-codes            X_THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES

#                                               X_THOR_DISABLE_MONGO_TEST_SERVER_PROTOCOL_VERSION_7
#X_AX_THOR_CHECK_DISABLE_MODTEST                   X_THOR_USE_MOD_TESTS_FLASE
#X_AX_THOR_CHECK_DISABLE_TIMEGM                    X_THOR_USE_TIMEGM_FLASE
#                                               X_THOR_DISABLE_POSTGRESS_TEST_SERVER_NOT_INSTALLED

AX_THOR_FUNC_TERM_BUILD([THORSDB], [src/MySQL/MySQLConfig.h:localconfig.h.in src/ThorsDB/ThorsDBConfig.h:localconfig.h.in src/ThorsDBCommon/ThorsDBConfig.h:localconfig.h.in src/Postgres/PostgresConfig.h:localconfig.h.in src/ThorsDBBuild/ThorsDBConfig.h:localconfig.h.in src/ThorsDBBuildTest/ThorsDBConfig.h:localconfig.h.in src/Mongo/MongoConfig.h:localconfig.h.in])

AC_OUTPUT
