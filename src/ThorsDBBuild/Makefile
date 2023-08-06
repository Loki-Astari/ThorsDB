
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsDB.lib
DEFER_LIBS					= ThorsDB ThorsDBCommon ThorsMySQL ThorsPostgres ThorsMongo
LDLIBS_MSYS_NT				= -lws2_32 -lwsock32 -lyaml
LDLIBS_MINGW64_NT			= -lws2_32 -lwsock32 -lyaml

LDLIBS_EXTERN_BUILD			= snappy crypto ThorSerialize
LINK_LIBS					+= ThorsSocket
UNITTEST_LDLIBS				+= $(LDLIBS_$(PLATFORM))
LDLIBS						+= $(LDLIBS_$(PLATFORM))

include $(THORSANVIL_ROOT)/build/tools/Makefile

