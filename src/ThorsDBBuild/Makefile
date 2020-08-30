
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsDB.lib
DEFER_LIBS					= ThorsDB ThorsDBCommon ThorsMySQL ThorsPostgress ThorsMongo
LDLIBS						= -lssl -lcrypto
LDFLAGS						= $(crypto_ROOT_LIBDIR)

include $(THORSANVIL_ROOT)/build/tools/Makefile

