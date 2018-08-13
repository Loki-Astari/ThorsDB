
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../SQL/Connection.h"
#include "ConectReader.h"
#include "RespPackageERR.h"
#include "test/MockStream.h"

using ThorsAnvil::SQL::Options;
using ThorsAnvil::MySQL::ConectReader;
using ThorsAnvil::MySQL::RespPackageERR;

TEST(RespPackageERRTest, DefaultERRMessage)
{
    char                buffer[] = "\x23\x00" "A12345Plop";
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageERR      eof(0xFF, reader);

    EXPECT_FALSE(eof.isOK());
    EXPECT_TRUE(eof.isError());
    EXPECT_FALSE(eof.isEOF());
    EXPECT_TRUE(eof.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << eof;

    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Plop)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);
    auto findErrorCode = message.str().find("errorCode(35)");   // 0x23 first two byte in buffer
    EXPECT_NE(std::string::npos, findErrorCode);
    auto findHash = message.str().find("hash(A)");              // a third character in buffer
    EXPECT_NE(std::string::npos, findHash);
    auto findState = message.str().find("stateSQL(12345)");     // next 5 bytes
    EXPECT_NE(std::string::npos, findState);
}

