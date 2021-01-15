
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 60

TARGET						= ThorsDBCommon.defer
DEFER_NAME					= ThorsDB
LDLIBS_EXTERN_BUILD			= crypto
UNITTEST_LDLIBS				= -lssl
UNITTEST_LINK_LIBS			= ThorsSocket ThorsLogging
NO_HEADER					= 1

include $(THORSANVIL_ROOT)/build/tools/Makefile

