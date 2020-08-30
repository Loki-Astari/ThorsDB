
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 73

TARGET						= ThorsMySQL.defer
DEFER_NAME					= ThorsDB
CXXFLAGS					= $(crypto_ROOT_INCDIR)
UNITTEST_LINK_LIBS			= ThorsDB ThorsDBCommon
UNITTEST_LDLIBS				= $(crypto_ROOT_LIBDIR) -lssl -lcrypto
NO_HEADER					= 1

include $(THORSANVIL_ROOT)/build/tools/Makefile

