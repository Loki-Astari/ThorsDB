
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMongo.defer
NAMESPACE					= ThorsAnvil::DB::Mongo
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= MagicEnum snappy crypto ThorSerialize
UNITTEST_LINK_LIBS			= ThorsSocket UnitTestThorsDB UnitTestThorsDBCommon

CRYPTO_WARNINGS_Linux		= -Wno-missing-braces
CRYPTO_WARNINGS				= $(CRYPTO_WARNINGS_$(PLATFORM)) -Wno-deprecated-declarations
LDLIBS_MSYS_NT				= -lws2_32 -lwsock32 -lyaml
LDLIBS_MINGW64_NT			= -lws2_32 -lwsock32 -lyaml

# TODO FIX
FILE_WARNING_FLAGS			+= -Wno-dollar-in-identifier-extension
# TODO FIX
# After ThorsSerialized has been published with homebrew (2.2.23)
# Then this flag can be removed.
FILE_WARNING_FLAGS			+= -Wno-unused-parameter


FILE_WARNING_FLAGS			+= $(CRYPTO_WARNINGS) -Wno-missing-braces
UNITTEST_LDLIBS				+= $(LDLIBS_$(PLATFORM))
LDLIBS						+= $(LDLIBS_$(PLATFORM))


include $(THORSANVIL_ROOT)/build/tools/Makefile

