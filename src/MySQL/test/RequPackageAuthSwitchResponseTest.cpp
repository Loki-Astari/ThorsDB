
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../ThorsDB/Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RequPackageAuthSwitchResp.h"
#include "test/MockStream.h"

using ThorsAnvil::DB::Access::Options;
using ThorsAnvil::DB::MySQL::ConectReader;
using ThorsAnvil::DB::MySQL::ConectWriter;
using ThorsAnvil::DB::MySQL::RespPackageAuthSwitchRequest;
using ThorsAnvil::DB::MySQL::RequPackageAuthSwitchResponse;

extern std::map<std::string, std::string>      options;

TEST(RequPackageAuthSwitchResponseTest, DefaultOKMessage)
{
    // See test/RespPackageAuthSwitchRequestTest.cpp
    // To show this is OK to create the RespPackageAuthSwitchRequest
    // This Way.
    char                buffer[] =  "mysql_native_password\x00"
                                    "ABCDEFGH\x00";
    unsigned char       output[500];
    MockStream          stream(buffer, sizeof(buffer) - 1, output);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);
    RespPackageAuthSwitchRequest      switchReq(0xFE, reader);


    RequPackageAuthSwitchResponse   authSwitch("test", "test", options, "test", switchReq.getPluginData());

    std::stringstream  message;
    message << authSwitch;

    auto findHumanMessage = message.str().find("humanMessage(RequPackageAuthSwitchResponse)");
    EXPECT_NE(std::string::npos, findHumanMessage);

    auto findauthResponse = message.str().find("authResponse(0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 )");
    EXPECT_NE(std::string::npos, findauthResponse);


    ConectWriter        writer(stream);
    authSwitch.build(writer);
    EXPECT_EQ(stream.writLen(), 8);
}

