
#include <gtest/gtest.h>
#include "ThorsSocket/SocketStream.h"
#include "ThorsSocket/Socket.h"
#include "HandShake.h"
#include "ThorsCrypto/scram.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/CustomSerialization.h"
#include <sstream>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(MongoConnectTest, CreateReply)
{
    ThorsAnvil::ThorsIO::ConnectSocket       socket("localhost", 27017);
    socket.makeSocketNonBlocking();
    ThorsAnvil::ThorsIO::IOSocketStream      stream(socket);

    HandShake           handShake("Test App");
    Op_QueryHandShake   handShakeMessage(handShake);

    stream << handShakeMessage;
    stream.flush();

    Op_ReplyHandShake   reply;
    stream >> reply;

    HandShakeReplyDoc const&    handShakeReply = reply.getDocument(0);
    std::cerr << ThorsAnvil::Serialize::jsonExporter(handShakeReply) << "\n";

    EXPECT_EQ(handShakeReply.code,      0);
    EXPECT_EQ(handShakeReply.errmsg,    "");
    EXPECT_EQ(handShakeReply.codeName,  "");
    ASSERT_EQ(handShakeReply.ok,        1);


    ThorsAnvil::Crypto::ScramClientSha256   client("loki"s);

    AuthInit            authInit("thor"s, "SCRAM-SHA-256"s, client.getFirstMessage());
    Op_MsgAuthInit      authInitMessage(authInit);

    stream << authInitMessage;
    stream.flush();

    AuthReply               authReply;
    Op_MsgAuthReply         authReplyMessage(authReply);
    stream >> authReplyMessage;

    std::cerr << ThorsAnvil::Serialize::jsonExporter(authReply) << "\n";;
    //EXPECT_EQ(authReply.code,      0);  // 185952104
    EXPECT_EQ(authReply.errmsg,    "");
    EXPECT_EQ(authReply.codeName,  "");
    ASSERT_EQ(authReply.ok,        1);

    std::cerr << "Server Message: " << authReply.payload.data << "\n";
    std::cerr << "Expected:       " << "r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,s=4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==,i=15000\n";
    std::string proof = client.getProofMessage("underworldSA0", authReply.payload.data);
    std::cerr << "Proof:          " << proof << "\n";
    std::cerr << "Expected:       " << "c=biws,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,p=h8+KRHxFHkrvC3t6Cq6KVLAt4mlBP/V6JrtTLMKvW/4=\n";

    AuthCont        authCont(authReply.conversationId, "thor", std::move(proof));
    Op_MsgAuthCont  authContMessage(authCont);

    stream << authContMessage;
    stream.flush();

    AuthReply               authReply2;
    Op_MsgAuthReply         authReplyMessage2(authReply2);
    stream >> authReplyMessage2;

    std::cerr << ThorsAnvil::Serialize::jsonExporter(authReply2) << "\n";;
    //EXPECT_EQ(authReply2.code,      0);  // 185952104
    EXPECT_EQ(authReply2.errmsg,    "");
    EXPECT_EQ(authReply2.codeName,  "");
    ASSERT_EQ(authReply2.ok,        1);

    AuthCont        authCont2(authReply.conversationId, "thor"s, ""s);
    Op_MsgAuthCont  authContMessage2(authCont2);

    stream << authContMessage2;
    stream.flush();

    AuthReply               authReply3;
    Op_MsgAuthReply         authReplyMessage3(authReply3);
    stream >> authReplyMessage;

    std::cerr << ThorsAnvil::Serialize::jsonExporter(authReply3) << "\n";;
    //EXPECT_EQ(authReply3.code,      0);  // 185952104
    EXPECT_EQ(authReply3.errmsg,    "");
    EXPECT_EQ(authReply3.codeName,  "");
    ASSERT_EQ(authReply3.ok,        1);

}

