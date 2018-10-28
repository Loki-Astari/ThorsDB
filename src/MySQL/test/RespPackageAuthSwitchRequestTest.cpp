
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../SQL/Connection.h"
#include "ConectReader.h"
#include "RespPackageAuthSwitchRequest.h"
#include "test/MockStream.h"

using ThorsAnvil::SQL::Options;
using ThorsAnvil::MySQL::ConectReader;
using ThorsAnvil::MySQL::RespPackageAuthSwitchRequest;

TEST(RespPackageAuthSwitchRequestTest, DefaultAuthSwitchRequestMessage)
{
    char                buffer[] =  "SmartPlugin\x00"
                                    "ABCDEFGH\x00";
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageAuthSwitchRequest      switchRequest(0xFE, reader);

    EXPECT_FALSE(switchRequest.isOK());
    EXPECT_FALSE(switchRequest.isError());
    EXPECT_FALSE(switchRequest.isEOF());
    EXPECT_TRUE(switchRequest.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << switchRequest;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);

    auto findpluginName = message.str().find("pluginName(SmartPlugin)");
    EXPECT_NE(std::string::npos, findpluginName);
    auto findpluginData = message.str().find("pluginData(0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 )");
    EXPECT_NE(std::string::npos, findpluginData);

    EXPECT_TRUE(reader.isEmpty());
}

