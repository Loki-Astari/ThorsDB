
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

    Op_MsgAuthInit      authInitMessage(AuthInit("thor"s, "SCRAM-SHA-256"s, client.getFirstMessage()));

    stream << authInitMessage;
    stream.flush();

    Op_MsgAuthReply         authReplyMessage;
    stream >> authReplyMessage;

    ASSERT_EQ(authReplyMessage.getDocument<0>().ok,        1);

    Op_MsgAuthCont  authContMessage(AuthCont(authReplyMessage.getDocument<0>().conversationId, "thor", client.getProofMessage("underworldSA0", authReplyMessage.getDocument<0>().payload.data)));

    stream << authContMessage;
    stream.flush();

    stream >> authReplyMessage;

    ASSERT_EQ(authReplyMessage.getDocument<0>().ok,        1);

    Op_MsgAuthCont  authContMessage2(AuthCont(authReplyMessage.getDocument<0>().conversationId, "thor"s, ""s));

    stream << authContMessage2;
    stream.flush();

    stream >> authReplyMessage;

    ASSERT_EQ(authReplyMessage.getDocument<0>().ok,        1);

    Op_QueryListDataBases   listDatabasesMessage;

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

