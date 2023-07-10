
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 60

TARGET						= ThorsDBCommon.defer
NAMESPACE					= ThorsAnvil::DB::Common
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= $(crypto_ROOT_LIB) $(ThorSerialize_ROOT_LIB)
UNITTEST_LINK_LIBS			= ThorsSocket

include $(THORSANVIL_ROOT)/build/tools/Makefile

