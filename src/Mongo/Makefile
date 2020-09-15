
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 60

TARGET						= ThorsMongo.defer
DEFER_NAME					= ThorsDB
NO_HEADER					= 1
LDLIBS_EXTERN_BUILD			= ThorsSerialize crypto
UNITTEST_LINK_LIBS			= UnitTestThorsDB UnitTestThorsDBCommon
UNITTEST_LDLIBS				+= -lssl
CXXFLAGS					+= -Wno-dollar-in-identifier-extension

%/Op_Msg.o:					CXXFLAGS += -Wno-unknown-pragmas
%/Op_Query.o:				CXXFLAGS += -Wno-unknown-pragmas
%/Op_Reply.o:				CXXFLAGS += -Wno-unknown-pragmas
%/HandShake.o:				CXXFLAGS += -Wno-unknown-pragmas
%/MsgHeaderTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/Op_MsgTest.o:				CXXFLAGS += -Wno-deprecated-declarations -Wno-unknown-pragmas
%/Op_QueryTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/HandShakeTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/MongoConnectTest.o:		CXXFLAGS += -Wno-deprecated-declarations

include $(THORSANVIL_ROOT)/build/tools/Makefile

