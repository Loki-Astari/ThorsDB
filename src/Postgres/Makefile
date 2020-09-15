
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 70

TARGET						= ThorsPostgres.defer
DEFER_NAME					= ThorsDB
LDLIBS_EXTERN_BUILD			= crypto
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon
UNITTEST_LDLIBS				= -lssl
NO_HEADER					= 1

%/ConectReader.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/ConectWriter.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/Connection.o:				FILE_WARNING_FLAGS += -Wno-unknown-pragmas -Wno-deprecated-declarations

include $(THORSANVIL_ROOT)/build/tools/Makefile

