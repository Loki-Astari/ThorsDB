
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMongo.defer
NAMESPACE					= ThorsAnvil::DB::Mongo
DEFER_NAME					= ThorsDB
NO_HEADER					= 1

LDLIBS_EXTERN_BUILD			= $(snappy_ROOT_LIB) $(crypto_ROOT_LIB) $(ThorSerialize_ROOT_LIB)
UNITTEST_LINK_LIBS			= ThorsSocket UnitTestThorsDB UnitTestThorsDBCommon

CRYPTO_WARNINGS_Darwin		= -Wno-deprecated-declarations
CRYPTO_WARNINGS_Linux		= -Wno-missing-braces
CRYPTO_WARNINGS				= $(CRYPTO_WARNINGS_$(PLATFORM))
FILE_WARNING_FLAGS			+= $(CRYPTO_WARNINGS)
# TODO FIX
FILE_WARNING_FLAGS			+= -Wno-dollar-in-identifier-extension

include $(THORSANVIL_ROOT)/build/tools/Makefile

