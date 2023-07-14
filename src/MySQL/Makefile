
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMySQL.defer
NAMESPACE					= ThorsAnvil::DB::MySQL
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= $(crypto_ROOT_LIB) $(ThorSerialize_ROOT_LIB)
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon ThorsSocket

include $(THORSANVIL_ROOT)/build/tools/Makefile

