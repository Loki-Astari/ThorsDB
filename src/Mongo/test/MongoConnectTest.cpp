
#include "MongoConfig.h"
#include <gtest/gtest.h>
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "CmdAdm_Auth.h"
#include "CmdAdm_ListDataBases.h"
#include "ThorsCrypto/scram.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/CustomSerialization.h"
#include <sstream>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(MongoConnectTest, CreateReply)
{
    // Connect
    ThorsAnvil::Crypto::ScramClientSha256   client(THOR_TESTING_MONGO_USER);
    ThorsAnvil::ThorsIO::ConnectSocket      socket(THOR_TESTING_MONGO_HOST, 27017);
    ThorsAnvil::ThorsIO::IOSocketStream     stream(socket);
    socket.makeSocketNonBlocking();

    // Send Handshake
    stream << CmdAdm_HandShake("Test App") << std::flush;

    CmdAdm_HandShakeReply   handShakeReplyMessage;
    stream >> handShakeReplyMessage;

    ASSERT_TRUE(handShakeReplyMessage.isOk());

    // Send Auth Init: We can use SHA-256 Send scram package
    stream << Op_MsgAuthInit({}, AuthInit(THOR_TESTING_MONGO_DB, "SCRAM-SHA-256"s, client.getFirstMessage())) << std::flush;

    Op_MsgAuthReply         authInitReplyMessage;
    stream >> authInitReplyMessage;

    AuthReply const&    authInitReply = authInitReplyMessage.getAction();
    ASSERT_EQ(authInitReply.ok,     1);

    // Send Auth Cont: Send proof we know the password
    stream << Op_MsgAuthCont({}, AuthCont(authInitReply.conversationId, THOR_TESTING_MONGO_DB, client.getProofMessage(THOR_TESTING_MONGO_PASS, authInitReply.payload.data))) << std::flush;

    Op_MsgAuthReply         authContReplyMessage;
    stream >> authContReplyMessage;

    AuthReply const&    authContReply = authContReplyMessage.getAction();
    ASSERT_EQ(authContReply.ok,     1);

    // Send Auth Cont 2: Send the DB Info
    stream << Op_MsgAuthCont({}, AuthCont(authContReply.conversationId, THOR_TESTING_MONGO_DB, ""s)) << std::flush;

    Op_MsgAuthReply         authContReply2Message;
    stream >> authContReply2Message;

    AuthReply const&    authContReply2 = authContReply2Message.getAction();
    ASSERT_EQ(authContReply.ok,     1);
    // If everything is OK then we should now be connected.


    // Send Command to prove authentication worked and we have an open and working connection:
    stream << CmdAdm_ListDataBases{} << std::flush;

    //ListDataBaseReply           listDBReply;
    CmdAdm_ListDataBasesReply   listOfDatabases;//(listDBReply);
    stream >> listOfDatabases;

    if (listOfDatabases)
    {

        std::cerr << "ListDB TS:   " << listOfDatabases.reply.totalSize << "\n";
        std::cerr << "ListDB DB: [";
        for (auto const& db: listOfDatabases.reply.databases)
        {
            std::cerr << "{Name: " << db.name << ", Size: " << db.sizeOnDisk << ", Empty: " << db.empty << "}";
        }
        std::cerr << "]\n";
    }
    else
    {
        std::cerr << "Failure retrieving DB List: " << listOfDatabases.getHRErrorMessage() << "\n";
    }
}

