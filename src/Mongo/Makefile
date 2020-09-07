
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMongo.defer
DEFER_NAME					= ThorsDB
NO_HEADER					= 1
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon

%/MsgHeaderTest.o:			CXXFLAGS += -Wno-deprecated-declarations

include $(THORSANVIL_ROOT)/build/tools/Makefile

