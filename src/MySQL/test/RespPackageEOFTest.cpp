
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../SQL/Connection.h"
#include "ConectReader.h"
#include "RespPackageEOF.h"
#include "test/MockStream.h"

using ThorsAnvil::DB::SQL::Options;
using ThorsAnvil::DB::MySQL::ConectReader;
using ThorsAnvil::DB::MySQL::RespPackageEOF;

TEST(RespPackageEOFTest, DefaultEOFMessage)
{
    char                buffer[] = "\x01\x00\x03\x00";
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageEOF      eof(0xFE, reader);

    EXPECT_FALSE(eof.isOK());
    EXPECT_FALSE(eof.isError());
    EXPECT_TRUE(eof.isEOF());
    EXPECT_TRUE(eof.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_EQ(eof.getStatusFlag(), 0x03);

    std::stringstream message;
    message << eof;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage()"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);
    auto findWarning = message.str().find("warningCount(0x00000001)");
    EXPECT_NE(std::string::npos, findWarning);
    auto findStatus = message.str().find("statusFlag(0x00000003)");
    EXPECT_NE(std::string::npos, findStatus);

    EXPECT_TRUE(reader.isEmpty());
}

