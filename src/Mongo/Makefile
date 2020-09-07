
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMongo.defer
DEFER_NAME					= ThorsDB
NO_HEADER					= 1
LDLIBS_EXTERN_BUILD			= ThorsSerialize
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon

%/MsgHeaderTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/Op_Msg.o:					CXXFLAGS += -Wno-unknown-pragmas

include $(THORSANVIL_ROOT)/build/tools/Makefile

