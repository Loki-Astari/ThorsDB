
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 75

TARGET						= ThorsPostgres.defer
DEFER_NAME					= ThorsDB
CXXFLAGS					= $(crypto_ROOT_INCDIR)
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon
UNITTEST_LDLIBS				= $(crypto_ROOT_LIBDIR) -lssl -lcrypto
NO_HEADER					= 1

%/ConectReader.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/ConectWriter.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/Connection.o:				FILE_WARNING_FLAGS += -Wno-unknown-pragmas -Wno-deprecated-declarations

include $(THORSANVIL_ROOT)/build/tools/Makefile

