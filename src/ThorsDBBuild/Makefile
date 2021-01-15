
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsDB.lib
DEFER_LIBS					= ThorsDB ThorsDBCommon ThorsMySQL ThorsPostgress ThorsMongo
LINK_LIBS					= ThorsSocket ThorsLogging
LDLIBS_EXTERN_BUILD			= ThorsSerialize crypto
LDLIBS						= -lssl

include $(THORSANVIL_ROOT)/build/tools/Makefile

