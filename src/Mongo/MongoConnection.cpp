#include "MongoConnection.h"

#include "CmdAdm_Auth.h"
#include "ThorsCrypto/scram.h"

using namespace ThorsAnvil::DB::Mongo;

// This registers the name "mongo" to create a "MongoConnection" object.
// Thus when you use:
//
//  ThorsAnvil::DB::Access::Connection  connection("mongo://<host>[:<port>]", <username>, <password>, <database>);
//  -> This will create a connection to a "mongo" data base.
//  -> if port is provided then it will be passed as a paraner. If not provided then 0 will be passed and
//     we should use the default mongo port (27017).
//
//  Pass an "AppName" in the options parameter (to be nice).
//  If none supplied then we will use "ThorsAnvil::Mongo Lib v1.0"
ThorsAnvil::DB::Access::Lib::ConnectionCreatorRegister<MongoConnection>    mongoConnection("mongo");

MongoConnection::MongoConnection(
                            std::string const& host, int port,
                            std::string const& username,
                            std::string const& password,
                            std::string const& database,
                            ThorsAnvil::DB::Access::Options const& options)
    : ConnectionProxy()
    , socket(host, port == 0 ? 27017 : port)
    , stream(socket)
    , dbName(database)
{
    using std::string_literals::operator""s;

    socket.makeSocketNonBlocking();

    // Connect
    ThorsAnvil::Crypto::ScramClientSha256   client(username);

    // Send Handshake
    auto findAppName = options.find("AppName");
    std::string const& appName = findAppName == options.end() ? "ThorsAnvil::Mongo Lib v1.0" : findAppName->second;
    stream << CmdAdm_HandShake(appName) << std::flush;

    CmdAdm_HandShakeReply   handShakeReply;
    stream >> handShakeReply;

    if (handShakeReply.handshake.ok != 1)
    {
        ThorsLogAndThrowCritical("ThorsAnvil::DB::Mongo::MongoConnection",
                                 "MongoConnection",
                                 "Handshake Request Failed: ",
                                 "Code: ", handShakeReply.handshake.codeName,
                                 "Msg:  ", handShakeReply.handshake.errmsg);
    }

    // Send Auth Init: We can use SHA-256 Send scram package
    stream << Op_MsgAuthInit({}, AuthInit(database, "SCRAM-SHA-256"s, client.getFirstMessage())) << std::flush;

    Op_MsgAuthReply         authInitReply;
    stream >> authInitReply;

    if (authInitReply.getDocument<0>().ok != 1)
    {
        ThorsLogAndThrowCritical("ThorsAnvil::DB::Mongo::MongoConnection",
                                 "MongoConnection",
                                 "Handshake FirstMessage: ",
                                 "Code: ", authInitReply.getDocument<0>().code,
                                 "Name: ", authInitReply.getDocument<0>().codeName,
                                 "Msg:  ", authInitReply.getDocument<0>().errmsg);
    }

    // Send Auth Cont: Send proof we know the password
    stream << Op_MsgAuthCont({}, AuthCont(authInitReply.getDocument<0>().conversationId,
                                      database,
                                      client.getProofMessage(password, authInitReply.getDocument<0>().payload.data)
                                     )
                            ) << std::flush;

    Op_MsgAuthReply         authContReply;
    stream >> authContReply;

    if (authContReply.getDocument<0>().ok != 1)
    {
        ThorsLogAndThrowCritical("ThorsAnvil::DB::Mongo::MongoConnection",
                                 "MongoConnection",
                                 "Handshake Proof: ",
                                 "Code: ", authContReply.getDocument<0>().code,
                                 "Name: ", authContReply.getDocument<0>().codeName,
                                 "Msg:  ", authContReply.getDocument<0>().errmsg);
    }

    // Send Auth Cont 2: Send the DB Info
    stream << Op_MsgAuthCont({}, AuthCont(authContReply.getDocument<0>().conversationId,
                                      database,
                                      ""s
                                     )
                            ) << std::flush;

    Op_MsgAuthReply         authContReply2;
    stream >> authContReply2;

    if (authContReply2.getDocument<0>().ok != 1)
    {
        ThorsLogAndThrowCritical("ThorsAnvil::DB::Mongo::MongoConnection",
                                 "MongoConnection",
                                 "Handshake DB Connect: ",
                                 "Code: ", authContReply2.getDocument<0>().code,
                                 "Name: ", authContReply2.getDocument<0>().codeName,
                                 "Msg:  ", authContReply2.getDocument<0>().errmsg);
    }
}

std::unique_ptr<ThorsAnvil::DB::Access::Lib::StatementProxy> MongoConnection::createStatementProxy(std::string const& /*statement*/)
{
    return nullptr;
}

int MongoConnection::getSocketId() const
{
    return socket.getSocketId();
}

void MongoConnection::setYield(std::function<void()>&& readYield, std::function<void()>&& writeYield)
{
    socket.setYield(std::move(readYield), std::move(writeYield));
}

MongoConnection::operator bool()
{
    return static_cast<bool>(stream);
}

CursorId MongoConnection::getLastOpenCursor(bool close)
{
    CursorId        result = lastCursor;
    lastCursor             = close ? -1 : lastCursor;
    return result;
}

std::vector<CursorId> MongoConnection::getAllOpenCursors(bool close)
{
    std::vector<CursorId>   result;
    for (auto const& v: openCursors)
    {
        result.emplace_back(v.first);
    }
    if (close)
    {
        openCursors.clear();
    }
    return result;
}

MongoConnection::CursorInfo MongoConnection::getCursorInfo(CursorId cursor) const
{
    auto find = openCursors.find(cursor);
    if (find == openCursors.end())
    {
        return CursorInfo{};
    }
    return find->second;
}
