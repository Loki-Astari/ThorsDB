
#include "MongoConfig.h"
#include <gtest/gtest.h>
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "HandShake.h"
#include "AdmCmdListDatabases.h"
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
    stream << Op_QueryHandShake("Test App") << std::flush;

    Op_ReplyHandShake   handShakeReplyMessage;
    stream >> handShakeReplyMessage;

    ASSERT_TRUE(handShakeReplyMessage.isOk());
    HandShakeReplyDoc const&    handShakeReply = handShakeReplyMessage.getDocument(0);

    // Send Auth Init: We can use SHA-256 Send scram package
    stream << Op_MsgAuthInit(AuthInit(THOR_TESTING_MONGO_DB, "SCRAM-SHA-256"s, client.getFirstMessage())) << std::flush;

    Op_MsgAuthReply         authInitReplyMessage;
    stream >> authInitReplyMessage;

    AuthReply const&    authInitReply = authInitReplyMessage.getDocument<0>();
    ASSERT_EQ(authInitReply.ok,     1);

    // Send Auth Cont: Send proof we know the password
    stream << Op_MsgAuthCont(AuthCont(authInitReply.conversationId, THOR_TESTING_MONGO_DB, client.getProofMessage(THOR_TESTING_MONGO_PASS, authInitReply.payload.data))) << std::flush;

    Op_MsgAuthReply         authContReplyMessage;
    stream >> authContReplyMessage;

    AuthReply const&    authContReply = authContReplyMessage.getDocument<0>();
    ASSERT_EQ(authContReply.ok,     1);

    // Send Auth Cont 2: Send the DB Info
    stream << Op_MsgAuthCont(AuthCont(authContReply.conversationId, THOR_TESTING_MONGO_DB, ""s)) << std::flush;

    Op_MsgAuthReply         authContReply2Message;
    stream >> authContReply2Message;

    AuthReply const&    authContReply2 = authContReply2Message.getDocument<0>();
    ASSERT_EQ(authContReply.ok,     1);
    // If everything is OK then we should now be connected.


    // Send Command to prove authentication worked and we have an open and working connection:
    stream << Op_QueryAdmCmdListDataBases{} << std::flush;

    Op_ReplAdmCmdListDataBases    listOfDatabases;
    stream >> listOfDatabases;

    if (listOfDatabases)
    {
        ListDataBaseReply const&    listDBReply = listOfDatabases.getDocument(0);

        std::cerr << "ListDB TS:   " << listDBReply.totalSize << "\n";
        std::cerr << "ListDB DB: [";
        for (auto const& db: listDBReply.databases)
        {
            std::cerr << "{Name: " << db.name << ", Size: " << db.sizeOnDisk << ", Empty: " << db.empty << "}";
        }
        std::cerr << "]\n";
    }
    else
    {
        std::cerr << "Failure retrieving DB List: " << listOfDatabases.getFailureMessage()
                  << "  Code: " << listOfDatabases.getFailureCode()
                  << "\n";
    }
}

