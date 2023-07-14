
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsDB.lib
DEFER_LIBS					= ThorsDB ThorsDBCommon ThorsMySQL ThorsPostgress ThorsMongo

LDLIBS_EXTERN_BUILD			= $(snappy_ROOT_LIB) $(crypto_ROOT_LIB) $(ThorSerialize_ROOT_LIB)
LINK_LIBS					= ThorsSocket

include $(THORSANVIL_ROOT)/build/tools/Makefile

