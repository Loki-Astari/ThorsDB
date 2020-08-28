
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../SQL/Connection.h"
#include "ConectReader.h"
#include "RespPackageERR.h"
#include "test/MockStream.h"

using ThorsAnvil::DB::SQL::Options;
using ThorsAnvil::DB::MySQL::ConectReader;
using ThorsAnvil::DB::MySQL::RespPackageERR;

TEST(RespPackageERRTest, DefaultERRMessage)
{
    char                buffer[] = "\x23\x00" "A12345Plop";
    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageERR      err(0xFF, reader);

    EXPECT_FALSE(err.isOK());
    EXPECT_TRUE(err.isError());
    EXPECT_FALSE(err.isEOF());
    EXPECT_TRUE(err.hasCap(CLIENT_PROTOCOL_41));

    std::stringstream message;
    message << err;

    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Plop)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);
    auto findErrorCode = message.str().find("errorCode(35)");   // 0x23 first two byte in buffer
    EXPECT_NE(std::string::npos, findErrorCode);
    auto findHash = message.str().find("hash(A)");              // a third character in buffer
    EXPECT_NE(std::string::npos, findHash);
    auto findState = message.str().find("stateSQL(12345)");     // next 5 bytes
    EXPECT_NE(std::string::npos, findState);

    EXPECT_TRUE(reader.isEmpty());
}

