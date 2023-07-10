
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 2

TARGET						= ThorsPostgres.defer
NAMESPACE					= ThorsAnvil::DB::Postgres
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= $(crypto_ROOT_LIB) $(ThorSerialize_ROOT_LIB)
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon ThorsSocket

%/ConectReader.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/ConectWriter.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/Connection.o:				FILE_WARNING_FLAGS += -Wno-unknown-pragmas -Wno-deprecated-declarations

include $(THORSANVIL_ROOT)/build/tools/Makefile

