
THORSANVIL_ROOT				?= $(realpath ../../)

TARGET						= ThorsMongo.defer
DEFER_NAME					= ThorsDB
NO_HEADER					= 1
LDLIBS_EXTERN_BUILD			= ThorsSerialize crypto
UNITTEST_LINK_LIBS			= ThorsSocket UnitTestThorsDB UnitTestThorsDBCommon ThorsLogging
UNITTEST_LDLIBS				+= -lssl
FILE_WARNING_FLAGS			+= -Wno-dollar-in-identifier-extension -Wno-unknown-pragmas

%/Op_MsgTest.o:				CXXFLAGS += -Wno-deprecated-declarations
%/MongoConnection.o:		CXXFLAGS += -Wno-deprecated-declarations

%/MongoConnectTest.o:		CXXFLAGS += -Wno-deprecated-declarations
%/HandShakeTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/Op_MsgHeaderTest.o:		CXXFLAGS += -Wno-deprecated-declarations
%/Op_QueryTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/Op_InsertTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/Op_DeleteTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/Op_GetMoreTest.o:			CXXFLAGS += -Wno-deprecated-declarations
%/Op_KillCursorsTest.o:		CXXFLAGS += -Wno-deprecated-declarations
%/Op_UpdateTest.o:			CXXFLAGS += -Wno-deprecated-declarations

include $(THORSANVIL_ROOT)/build/tools/Makefile

