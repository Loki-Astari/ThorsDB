
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMySQL.defer
DEFER_NAME					= ThorsDB
LDLIBS_EXTERN_BUILD			= crypto
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon ThorsSocket ThorsLogging
UNITTEST_LDLIBS				= -lssl
NO_HEADER					= 1

include $(THORSANVIL_ROOT)/build/tools/Makefile

