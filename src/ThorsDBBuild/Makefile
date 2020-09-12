
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsDB.lib
DEFER_LIBS					= ThorsDB ThorsDBCommon ThorsMySQL ThorsPostgress ThorsMongo
LDLIBS_EXTERN_BUILD			= crypto
LDLIBS						= -lssl

include $(THORSANVIL_ROOT)/build/tools/Makefile

