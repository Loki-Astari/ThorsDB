#include "DefaultMongoConnection.h"

#include "HandShake.h"
#include "ThorsCrypto/scram.h"


using namespace ThorsAnvil::DB::Mongo;

// ThorsAnvil::ThorsIO::ConnectSocket      socket;
// ThorsAnvil::ThorsIO::IOSocketStream     stream;
// ThorsAnvil::DB::Access::Lib::ConnectionProxy

DefaultMongoConnection::DefaultMongoConnection(
                            std::string const& host, int port,
                            std::string const& username,
                            std::string const& password,
                            std::string const& database,
                            ThorsAnvil::DB::Access::Options const& /*options*/)
    : ConnectionProxy()
    , socket(host, port)
    , stream(socket)
{
    using std::string_literals::operator""s;

    socket.makeSocketNonBlocking();

    // Connect
    ThorsAnvil::Crypto::ScramClientSha256   client(username);

    // Send Handshake
    stream << Op_QueryHandShake("Test App") << std::flush;

    Op_ReplyHandShake   handShakeReply;
    stream >> handShakeReply;

    if (handShakeReply.getDocument(0).ok != 1)
    {
        throw int(1);
    }

    // Send Auth Init: We can use SHA-256 Send scram package
    stream << Op_MsgAuthInit(AuthInit(std::string(database), "SCRAM-SHA-256"s, client.getFirstMessage())) << std::flush;

    Op_MsgAuthReply         authInitReply;
    stream >> authInitReply;

    if (authInitReply.getDocument<0>().ok != 1)
    {
        throw int(1);
    }

    // Send Auth Cont: Send proof we know the password
    stream << Op_MsgAuthCont(AuthCont(authInitReply.getDocument<0>().conversationId,
                                      std::string(database),
                                      client.getProofMessage(password, authInitReply.getDocument<0>().payload.data)
                                     )
                            ) << std::flush;

    Op_MsgAuthReply         authContReply;
    stream >> authContReply;

    if (authContReply.getDocument<0>().ok != 1)
    {
        throw int(1);
    }

    // Send Auth Cont 2: Send the DB Info
    stream << Op_MsgAuthCont(AuthCont(authContReply.getDocument<0>().conversationId,
                                      std::string(database),
                                      ""s
                                     )
                            ) << std::flush;

    Op_MsgAuthReply         authContReply2;
    stream >> authContReply2;

    if (authContReply2.getDocument<0>().ok != 1)
    {
        throw int(1);
    }
}

std::unique_ptr<ThorsAnvil::DB::Access::Lib::StatementProxy> DefaultMongoConnection::createStatementProxy(std::string const& /*statement*/)
{
    return nullptr;
}

int DefaultMongoConnection::getSocketId() const
{
    return socket.getSocketId();
}

void DefaultMongoConnection::setYield(std::function<void()>&& readYield, std::function<void()>&& writeYield)
{
    socket.setYield(std::move(readYield), std::move(writeYield));
}
