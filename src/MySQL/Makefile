
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMySQL.defer
DEFER_NAME					= ThorsDB
CXXFLAGS					= $(crypto_ROOT_INCDIR)
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon
UNITTEST_LDLIBS				= $(crypto_ROOT_LIBDIR) -lssl -lcrypto
NO_HEADER					= 1

include $(THORSANVIL_ROOT)/build/tools/Makefile

