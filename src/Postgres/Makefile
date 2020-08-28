
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 50

TARGET						= ThorPostgres.slib
LDLIBS						= -lcrypto
CXXFLAGS					= $(crypto_ROOT_INCDIR)
LDFLAGS						= $(crypto_ROOT_LIBDIR)
LINK_LIBS					= ThorSQL
UNITTEST_LINK_LIBS			= ThorSQL
NO_HEADER					= 1

%/ConectReader.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/ConectWriter.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/Connection.o:				FILE_WARNING_FLAGS += -Wno-unknown-pragmas

include $(THORSANVIL_ROOT)/build/tools/Makefile




