
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
    ThorsAnvil::ThorsIO::IOSocketStream      stream(socket);

#if 0

    Op_Query<HandShake>         handShake("admin.$cmd", "MongoTestUtility"s);
    std::cerr << make_hr(handShake);

    stream << handShake;
    stream.flush();
    Op_ReplyHandShake           reply;
    stream >> reply;

    std::cerr << make_hr(reply) << "\n\n";
    ThorsAnvil::Crypto::ScramClient client("loki");
    std::cerr << "FirstMessage: " << client.getFirstMessage() << "\n";
    Op_MsgAuthInit         authInit("thor"s, "SCRAM-SHA-1"s, client.getFirstMessage());
    stream << authInit;
    stream.flush();

    Op_MsgAuthInitReply    authReply;
    stream >> authReply;

    std::cerr << make_hr(authReply);
    AuthInitReply&          authReplyPayload = authReply.getDocument<0>();
    std::string             message(std::begin(authReplyPayload.payload.data), std::end(authReplyPayload.payload.data));
    std::cerr << "Reply: " << message << "\n";

    std::string proof = client.getProofMessage("underworldSA0", message);
    std::cerr << "SecondMessage: " << proof << "\n";

    Op_MsgAuthContinue      authContinue(authReplyPayload.conversationId, std::move(proof), "thor"s);
    stream << authContinue;
    stream.flush();
#endif


#if 1
#else
    stream >> std::noskipws;

    unsigned char c;
    std::string   text;
    int  loop = 0;
    std::cerr << "Reading Data\n";
    while(stream >> c)
    {
        std::cerr << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(c) << " ";
        text += ((c >= 20 && c < 127) ? c : '~');;

        ++loop;
        if (loop == 16)
        {
            std::cerr << "      " << text << "\n";
            text = "";
            loop = 0;
        }
    }
    std::cerr << std::string(' ', (16-loop)*3) << "      " << text << "\n";
#endif
}

#if 0
76 00 00 00 db 15 00 00 04 00 00 00 dd 07 00 00       v~~~~~~~~~~~~~~
00 00 00 00 00 61 00 00 00 01 6f 6b 00 00 00 00       ~~~~~a~~~~ok~~~~
00 00 00 00 00 02 65 72 72 6d 73 67 00 17 00 00       ~~~~~~errmsg~~~
00 41 75 74 68 65 6e 74 69 63 61 74 69 6f 6e 20       ~Authentication
66 61 69 6c 65 64 2e 00 10 63 6f 64 65 00 12 00       failed.~~code~~~
00 00 02 63 6f 64 65 4e 61 6d 65 00 15 00 00 00       ~~~codeName~~~~
41 75 74 68 65 6e 74 69 63 61 74 69 6f 6e 46 61       AuthenticationFa
69 6c 65 64 00 00

76 00 00 00     // Size 118
db 15 00 00     // Request ID
04 00 00 00     // Response To
dd 07 00 00     // Op_CODE; OP_MSG
OPMSG
00 00 00 00     // Flags
00              // Kind 0
BSON
61 00 00 00     // Size 97
    01 ok       00 00 00 00 00 00 00 00
    02 errmsg   17 00 00 00     Authentication failed
    10 code     12 00 00 00
    02 codeName 15 00 00 00     AuthenticationFailed
00

#endif


#if 0
d8 00 00 00 02 00 00 00   ?...?...........
00 00 00 00 dd 07 00 00 01 00 00 00 00 bf 00 00   ................
00 10 73 61 73 6c 43 6f 6e 74 69 6e 75 65 00 01   ..saslContinue..
00 00 00 05 70 61 79 6c 6f 61 64 00 78 00 00 00   ....payload.x...
00 63 3d 62 69 77 73 2c 72 3d 4a 53 79 52 48 44   .c=biws,r=JSyRHD
37 73 63 39 52 67 44 43 44 7a 4a 4a 4e 56 64 6b   7sc9RgDCDzJJNVdk
41 32 47 6c 53 65 4d 4a 50 56 35 6e 39 35 70 30   A2GlSeMJPV5n95p0
77 64 5a 7a 78 6a 50 74 37 7a 79 4c 45 4e 66 31   wdZzxjPt7zyLENf1
54 6f 38 68 59 54 62 4b 45 51 2c 70 3d 68 38 2b   To8hYTbKEQ,p=h8+
4b 52 48 78 46 48 6b 72 76 43 33 74 36 43 71 36   KRHxFHkrvC3t6Cq6
4b 56 4c 41 74 34 6d 6c 42 50 2f 56 36 4a 72 74   KVLAt4mlBP/V6Jrt
54 4c 4d 4b 76 57 2f 34 3d 10 63 6f 6e 76 65 72   TLMKvW/4=.conver
73 61 74 69 6f 6e 49 64 00 01 00 00 00 02 24 64   sationId......$d
62 00 05 00 00 00 74 68 6f 72 00 00 bd 34 27 d8   b.....thor...4'.

d8 00 00 00     // Size 216
02 00 00 00     // Request Id
00 00 00 00     // Response To
dd 07 00 00     // OP_CODE 2013: OP_MSG
Msg
01 00 00 00     // Flag
00              // Kind 0
BSON
bf 00 00 00     // Size
    10  saslContinue        01 00 00 00
    05  payload             78 00 00 00     00      c=biws,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,p=h8+KRHxFHkrvC3t6Cq6KVLAt4mlBP/V6JrtTLMKvW/4=
    10  conversationId      01 00 00 00
    02  $db                 05 00 00 00     thor
00
bd 34 27 d8

#endif

#if 0
75 00 00 00 91 15 00 00 02 00 00 00 dd 07 00 00       u~~~~~~~~~~~~~~
00 00 00 00 00 60 00 00 00 01 6f 6b 00 00 00 00       ~~~~~`~~~~ok~~~~
00 00 00 00 00 02 65 72 72 6d 73 67 00 21 00 00       ~~~~~~errmsg~!~~
00 4d 69 73 73 69 6e 67 20 65 78 70 65 63 74 65       ~Missing expecte
64 20 66 69 65 6c 64 20 22 70 61 79 6c 6f 61 64       d field "payload
22 00 10 63 6f 64 65 00 04 00 00 00 02 63 6f 64       "~~code~~~~~~cod
65 4e 61 6d 65 00 0a 00 00 00 4e 6f 53 75 63 68       eName~~~~~NoSuch
4b 65 79 00 00

75 00 00 00     // Size 117
91 15 00 00     // Requrst ID
02 00 00 00     // Response To
dd 07 00 00     // OpCode: 2013 Op_MSG
Msg
00 00 00 00     // Flags
00
BSON
60 00 00 00     // Size 96
    01 ok               00 00 00 00 00 00 00 00
    02 errmsg           21 00 00 00     Missing expected field "payload"
    10 code             04 00 00 00
    02 codeName         0a 00 00 00     NoSuchKey
00
#endif
#if 0

ac 00 00 00 66 15 00 00 59 34 00 00 dd 07 00 00       ~~~~f~~Y4~~~~~~
00 00 00 00 00 97 00 00 00 10 63 6f 6e 76 65 72       ~~~~~~~~~~conver
73 61 74 69 6f 6e 49 64 00 01 00 00 00 08 64 6f       sationId~~~~~~do
6e 65 00 00 05 70 61 79 6c 6f 61 64 00 5d 00 00       ne~~~payload~]~~
00 00 72 3d 66 79 6b 6f 2b 64 32 6c 62 62 46 67       ~~r=fyko+d2lbbFg
4f 4e 52 76 39 71 6b 78 64 61 77 4c 4a 72 63 34       ONRv9qkxdawLJrc4
73 6d 4f 55 6a 42 65 47 34 69 55 49 39 79 6b 37       smOUjBeG4iUI9yk7
75 39 71 4b 58 38 4e 46 67 4e 75 4c 2c 73 3d 79       u9qKX8NFgNuL,s=y
50 57 2f 70 65 79 32 6a 61 48 4a 2b 67 44 59 34       PW/pey2jaHJ+gDY4
4e 73 64 6c 77 3d 3d 2c 69 3d 31 30 30 30 30 01       Nsdlw==,i=10000~
6f 6b 00 00 00 00 00 00 00 f0 3f 00


ac 00 00 00     // Size 172
66 15 00 00     // RequestId
59 34 00 00     // Response TO
dd 07 00 00     // Op_Code: 2013 Op_MSG
Msg
00 00 00 00     // Flags
00              // Kind 0
BSON
97 00 00 00
    10  conversationId  01 00 00 00
    08  done            00
    05  payload         5d 00 00 00     00      r=fyko+d2lbbFgONRv9qkxdawLJrc4smOUjBeG4iUI9yk7u9qKX8NFgNuL,s=yPW/pey2jaHJ+gDY4Nsdlw==,i=10000
00

#endif

#if 0
80 01 00 00     // Size:    384
02 15 00 00     // requesstId
01 00 00 00     // Response To
01 00 00 00     // Op_Code: OP_REPLY
REPLY
08 00 00 00     // Flags:
00 00 00 00     // Cursor 1
00 00 00 00     // Cursor 2
00 00 00 00     // startingFrom
01 00 00 00     // numberReturned

BSON
5c 01 00 00     // Size:    348
    08  ismaster    01
    03  topologyVersion 
    2d 00 00 00
        07 processId        5f 48 aa 04 2d 1d fb 1d d7 05 2e 00
        12 counter          00 00 00 00 00 00 00 00
    00
    10  maxBsonObjectSize   00 00 00 01
    10  maxMessageSizeBytes 00 6c dc 02
    10  maxWriteBatchSize   a0 86 01 00
    09  localTime           38 97 dd 83 74 01 00 00
    10  logicalSessionTimeoutMinutes    1e 00 00 00
    10  connectionId        5f 00 00 00
    10  minWireVersion      00 00 00 00
    10  maxWireVersion      09 00 00 00
    08  readOnly            00
    04  saslSupportedMechs
    2d 00 00 00
        02  0   0c 00 00 00     SCRAM-SHA-1
        02  1   0e 00 00 00     SCRAM-SHA-256
    00
    01  ok                  00 00 00 00 00 00 f0 3f
00
#endif
