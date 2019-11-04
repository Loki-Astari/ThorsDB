
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorMySQL.lib
LDLIBS						= -lcrypto
CXXFLAGS					= $(crypto_ROOT_INCDIR)
LDFLAGS						= $(crypto_ROOT_LIBDIR)
LINK_LIBS					= ThorSQL
UNITTEST_LINK_LIBS			= ThorSQL

COVERAGE_REQUIRED			= 70

include $(THORSANVIL_ROOT)/build/tools/Makefile




