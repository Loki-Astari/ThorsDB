
#include <gtest/gtest.h>
#include "ThorMySQL.h"
#include "../SQL/Connection.h"
#include "ConectReader.h"
#include "RespPackageOK.h"
#include "test/MockStream.h"

using ThorsAnvil::DB::SQL::Options;
using ThorsAnvil::DB::MySQL::ConectReader;
using ThorsAnvil::DB::MySQL::RespPackageOK;

TEST(RespPackageOKTest, DefaultOKMessage)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x07\x08"          // warningFlags
                                    "Human Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_TRUE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_FALSE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_FALSE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000807)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, NotCLIENT_PROTOCOL_41)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "Human Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(0, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_FALSE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_FALSE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_FALSE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, Transaction)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "Human Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_TRANSACTIONS, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_FALSE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_TRUE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_FALSE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, Tracking)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x0EHuman Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_SESSION_TRACK, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_FALSE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_FALSE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, TrackingAndStateChange)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x0EHuman Readable"
                                    "\x08Twisting"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_SESSION_TRACK | SERVER_SESSION_STATE_CHANGED, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_FALSE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_FALSE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_TRUE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, Proto41Transactions)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x07\x08"          // warningFlags
                                    "Human Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41 | CLIENT_TRANSACTIONS, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_TRUE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_TRUE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_FALSE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000807)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, Proto41Session)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x07\x08"          // warningFlags
                                    "\x0EHuman Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41 | CLIENT_SESSION_TRACK, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_TRUE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_FALSE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000807)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, Proto41SessionState)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x07\x08"          // warningFlags
                                    "\x0EHuman Readable"
                                    "\x08QBCDEFGH"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41 | CLIENT_SESSION_TRACK | SERVER_SESSION_STATE_CHANGED, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_TRUE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_FALSE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_TRUE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000807)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, TransSession)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x0EHuman Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_TRANSACTIONS | CLIENT_SESSION_TRACK, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_FALSE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_TRUE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, TranSessionState)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x0EHuman Readable"
                                    "\x08PlopPlop"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_TRANSACTIONS | CLIENT_SESSION_TRACK | SERVER_SESSION_STATE_CHANGED, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_FALSE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_TRUE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_TRUE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000000)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, ProtTansSession)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x07\x08"          // warningFlags
                                    "\x0EHuman Readable"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41 | CLIENT_TRANSACTIONS | CLIENT_SESSION_TRACK, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_TRUE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_TRUE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_FALSE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000807)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

TEST(RespPackageOKTest, ProoTransSessionState)
{
    char                buffer[] =  "\xFC\x01\x02"      // affectedRows
                                    "\xFC\x03\x04"      // lastInsertID
                                    "\x05\x06"          // statusFlags
                                    "\x07\x08"          // warningFlags
                                    "\x0EHuman Readable"
                                    "\x08PlopPlop"
                                    ;

    MockStream          stream(buffer, sizeof(buffer) - 1);
    ConectReader        reader(stream);
    reader.initFromHandshake(CLIENT_PROTOCOL_41 | CLIENT_TRANSACTIONS | CLIENT_SESSION_TRACK | SERVER_SESSION_STATE_CHANGED, 0);

    RespPackageOK      ok(0x00, reader);

    EXPECT_TRUE(ok.isOK());
    EXPECT_FALSE(ok.isError());
    EXPECT_FALSE(ok.isEOF());
    EXPECT_TRUE(ok.hasCap(CLIENT_PROTOCOL_41));
    EXPECT_TRUE(ok.hasCap(CLIENT_TRANSACTIONS));
    EXPECT_TRUE(ok.hasCap(CLIENT_SESSION_TRACK));
    EXPECT_TRUE(ok.hasCap(SERVER_SESSION_STATE_CHANGED));

    std::stringstream message;
    message << ok;
    EXPECT_NE(message.str(), "");
    auto findHumanMassage = message.str().find("humanMessage(Human Readable)"); // The rest of the buffer
    EXPECT_NE(std::string::npos, findHumanMassage);


    auto findaffectedRows = message.str().find("affectedRows( 0x00000201)");
    EXPECT_NE(std::string::npos, findaffectedRows);
    auto findlastInsertID = message.str().find("lastInsertID( 0x00000403)");
    EXPECT_NE(std::string::npos, findlastInsertID);
    auto findstatusFlags = message.str().find("statusFlags( 0x00000605)");
    EXPECT_NE(std::string::npos, findstatusFlags);
    auto findwarningFlags = message.str().find("warningFlags( 0x00000807)");
    EXPECT_NE(std::string::npos, findwarningFlags);

    EXPECT_TRUE(reader.isEmpty());
}

