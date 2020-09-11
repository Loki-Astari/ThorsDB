
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

    Op_ReplyHandShake           reply;
    stream >> reply;

    std::cerr << make_hr(reply);

}

