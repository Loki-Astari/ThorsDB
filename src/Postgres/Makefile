
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 2

TARGET						= ThorsPostgres.defer
NAMESPACE					= ThorsAnvil::DB::Postgres
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= crypto ThorSerialize
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon ThorsSocket
LDLIBS_MSYS_NT				= -lws2_32 -lwsock32
LDLIBS_MINGW64_NT			= -lws2_32 -lwsock32

UNITTEST_LDLIBS				+= $(LDLIBS_$(PLATFORM))
LDLIBS						+= $(LDLIBS_$(PLATFORM))

%/Connection.o:				FILE_WARNING_FLAGS += -Wno-deprecated-declarations

include $(THORSANVIL_ROOT)/build/tools/Makefile

