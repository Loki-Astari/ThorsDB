
#include <gtest/gtest.h>
#include "SocketStream.h"
#include "Socket.h"
#include "HandShake.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

bool finished = false;

TEST(MongoConnectTest, CreateReply)
{
    Socket::ConnectSocket       socket("localhost", 27017);
    Socket::IOSocketStream      stream(socket,[](){if (finished){throw int(4);}}, [](){std::cerr << "N2\n";});


    Op_Query<HandShake>         handShake("admin.$cmd", "MongoTestUtility"s);
    std::cerr << make_hr(handShake);

    stream << handShake;
    stream.flush();
#if 1
    Op_ReplyHandShake           reply;
    stream >> reply;

    std::cerr << make_hr(reply);
#else
    stream >> std::noskipws;

    unsigned char c;
    std::string   text;
    int  loop = 0;
    std::cerr << "Reading Data\n";
    while(stream >> c)
    {
        finished = true;
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
#endif
}
#if 0
0000   02 00 00 00 45 00 00 c6 00 00 40 00 40 06 00 00   ....E.....@.@...
0010   7f 00 00 01 7f 00 00 01 e0 72 69 89 17 d1 cd 1d   .........ri.....
0020   4b be 8c 5f 80 18 18 e5 fe ba 00 00 01 01 08 0a   K.._............
0030   3f ee cc 0f 3f ee cc 0f 92 00 00 00 01 00 00 00   ?...?...........
0040   00 00 00 00 dd 07 00 00 01 00 00 00 00 79 00 00   .............y..
0050   00 10 73 61 73 6c 53 74 61 72 74 00 01 00 00 00   ..saslStart.....
0060   02 6d 65 63 68 61 6e 69 73 6d 00 0e 00 00 00 53   .mechanism.....S
0070   43 52 41 4d 2d 53 48 41 2d 32 35 36 00 05 70 61   CRAM-SHA-256..pa
0080   79 6c 6f 61 64 00 2c 00 00 00 00 6e 2c 2c 6e 3d   yload.,....n,,n=
0090   6c 6f 6b 69 2c 72 3d 4a 53 79 52 48 44 37 73 63   loki,r=JSyRHD7sc
00a0   39 52 67 44 43 44 7a 4a 4a 4e 56 64 6b 41 32 47   9RgDCDzJJNVdkA2G
00b0   6c 53 65 4d 4a 50 56 02 24 64 62 00 05 00 00 00   lSeMJPV.$db.....
00c0   74 68 6f 72 00 00 10 44 12 5c                     thor...D.\

92 00 00 00     Size
01 00 00 00     MessageID
00 00 00 00     Reply To
dd 07 00 00     Op_Code => 2013 OpMsg

01 00 00 00     Flags
BSON Sections:
00
79 00 00 00     => 121
    10  "saslStart\0"   01 00 00 00
    02  "mechanism\0"   0e 00 00 00     "SCRAM-SHA-256\0"
    05  "payload\0"     2c 00 00 00     00  "n,,n=loki,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV"
    02  "$db\0"         05 00 00 00     "thor\0"
00
10 44 12 5c
#endif

