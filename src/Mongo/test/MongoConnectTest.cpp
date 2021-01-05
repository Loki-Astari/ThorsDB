
#include <gtest/gtest.h>
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "HandShake.h"
#include "ListDatabases.h"
#include "ThorsCrypto/scram.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/CustomSerialization.h"
#include <sstream>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(MongoConnectTest, CreateReply)
{
    ThorsAnvil::Crypto::ScramClientSha256   client("loki"s);
    ThorsAnvil::ThorsIO::ConnectSocket      socket("localhost", 27017);
    ThorsAnvil::ThorsIO::IOSocketStream     stream(socket);
    socket.makeSocketNonBlocking();

    HandShake           handShake("Test App");
    Op_QueryHandShake   handShakeMessage(handShake);
    Op_ReplyHandShake   reply;
    AuthReply           authReply;

    stream << handShakeMessage;
    stream.flush();

    stream >> reply;

    HandShakeReplyDoc const&    handShakeReply = reply.getDocument(0);

    ASSERT_EQ(handShakeReply.ok,        1);

    AuthInit            authInit("thor"s, "SCRAM-SHA-256"s, client.getFirstMessage());
    Op_MsgAuthInit      authInitMessage(authInit);

    stream << authInitMessage;
    stream.flush();

    Op_MsgAuthReply         authReplyMessage(authReply);
    stream >> authReplyMessage;

    ASSERT_EQ(authReply.ok,        1);

    AuthCont        authCont(authReply.conversationId, "thor", client.getProofMessage("underworldSA0", authReply.payload.data));
    Op_MsgAuthCont  authContMessage(authCont);

    stream << authContMessage;
    stream.flush();

    AuthReply               authReply2;
    Op_MsgAuthReply         authReplyMessage2(authReply2);
    stream >> authReplyMessage2;

    ASSERT_EQ(authReply2.ok,        1);

    AuthCont        authCont2(authReply.conversationId, "thor"s, ""s);
    Op_MsgAuthCont  authContMessage2(authCont2);

    stream << authContMessage2;
    stream.flush();

    AuthReply               authReply3;
    Op_MsgAuthReply         authReplyMessage3(authReply3);
    stream >> authReplyMessage;

    ASSERT_EQ(authReply3.ok,        1);

    ListDataBases           listDatabases;
    Op_QueryListDataBases   listDatabasesMessage(listDatabases);

    stream << listDatabasesMessage;
    stream.flush();

    Op_ReplListDataBases    listOfDatabases;
    stream >> listOfDatabases;

    ListDataBaseReply const&    listDBReply = listOfDatabases.getDocument(0);

    std::cerr << "ListDB OK: "   << listDBReply.ok << "\n";
    std::cerr << "ListDB Code: " << listDBReply.code << "\n";
    std::cerr << "ListDB Err:  " << listDBReply.$err << "\n";
    std::cerr << "ListDB TS:   " << listDBReply.totalSize << "\n";
    std::cerr << "ListDB DB: [";
    for (auto const& db: listDBReply.databases)
    {
        std::cerr << "{Name: " << db.name << ", Size: " << db.sizeOnDisk << ", Empty: " << db.empty << "}";
    }
    std::cerr << "]\n";
}

