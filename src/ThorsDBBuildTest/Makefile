
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsDBTest.test
LDLIBS_EXTERN_BUILD			= ThorSerialize
NO_HEADER					= 1

LDLIBS_MSYS_NT				= -lws2_32 -lwsock32
LDLIBS_MINGW64_NT			= -lws2_32 -lwsock32

UNITTEST_LINK_LIBS			+= ThorsDB ThorsSocket
UNITTEST_LDLIBS				+= $(LDLIBS_$(PLATFORM))
LDLIBS						+= $(LDLIBS_$(PLATFORM))

include $(THORSANVIL_ROOT)/build/tools/Makefile

