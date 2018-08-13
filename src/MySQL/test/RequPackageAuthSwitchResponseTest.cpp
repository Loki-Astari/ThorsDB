
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../SQL/Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RequPackageAuthSwitchResp.h"
#include "test/MockStream.h"

using ThorsAnvil::SQL::Options;
using ThorsAnvil::MySQL::ConectReader;
using ThorsAnvil::MySQL::ConectWriter;
using ThorsAnvil::MySQL::RespPackageAuthSwitchRequest;
using ThorsAnvil::MySQL::RequPackageAuthSwitchResponse;

extern std::map<std::string, std::string>      options;

TEST(RequPackageAuthSwitchRespTest, DefaultOKMessage)
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


    RequPackageAuthSwitchResponse   authSwitch("test", "test", options, "test", switchReq);

    std::stringstream  message;
    message << authSwitch;

    auto findHumanMessage = message.str().find("humanMessage(RequPackageAuthSwitchResponse)");
    EXPECT_NE(std::string::npos, findHumanMessage);

    auto findauthResponse = message.str().find("authResponse(0x79 0xac 0x1a 0xbb 0xd4 0x5d 0x50 0x55 0x6d 0xcd 0x20 0xb0 0x25 0xb0 0x7d 0xb5 0x4f 0x2f 0x9f 0x56 )");
    EXPECT_NE(std::string::npos, findauthResponse);


    ConectWriter        writer(stream);
    authSwitch.build(writer);
    EXPECT_EQ(stream.writLen(), 20);
}

