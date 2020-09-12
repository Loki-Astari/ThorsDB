
#include <gtest/gtest.h>
#include "SocketStream.h"
#include "Socket.h"
#include "Op_QueryHandShake.h"
#include "Op_ReplyHandShake.h"

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

