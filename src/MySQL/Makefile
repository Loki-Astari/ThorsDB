
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMySQL.defer
NAMESPACE					= ThorsAnvil::DB::MySQL
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= crypto ThorSerialize
LDLIBS_MSYS_NT				= -lws2_32 -lwsock32
LDLIBS_MINGW64_NT			= -lws2_32 -lwsock32

UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon ThorsSocket
_LDLIBS						+= $(LDLIBS_$(PLATFORM))
UNITTEST_LDLIBS				+= $(LDLIBS_$(PLATFORM))

include $(THORSANVIL_ROOT)/build/tools/Makefile

