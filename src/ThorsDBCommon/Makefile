
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 60

TARGET						= ThorsDBCommon.defer
NAMESPACE					= ThorsAnvil::DB::Common
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= crypto ThorSerialize
LDLIBS_MSYS_NT				= -lws2_32 -lwsock32
LDLIBS_MINGW64_NT			= -lws2_32 -lwsock32

UNITTEST_LINK_LIBS			+= ThorsSocket
UNITTEST_LDLIBS				+= $(LDLIBS_$(PLATFORM))
LDLIBS						+= $(LDLIBS_$(PLATFORM))

include $(THORSANVIL_ROOT)/build/tools/Makefile

