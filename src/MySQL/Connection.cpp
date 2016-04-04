
#include "Connection.h"
#include "RespPackageHandShake.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RequPackageHandShakeResp.h"
#include "PrepareStatement.h"
#include "RespPackageOK.h"

using namespace ThorsAnvil::MySQL;

DefaultMySQLConnection::DefaultMySQLConnection(
                            std::string const& host, int port,
                            std::string const& username,
                            std::string const& password,
                            std::string const& database,
                            ThorsAnvil::SQL::Options const& options)
    : stream(host, port)
    , buffer(stream, true)
    , reader(buffer)
    , writer(buffer)
    , connection(username, password, database, options, reader, writer)
{}
std::unique_ptr<ThorsAnvil::SQL::StatementProxy>
DefaultMySQLConnection::createStatementProxy(std::string const& statement, ThorsAnvil::SQL::StatementType /*type*/)
{
    std::unique_ptr<ThorsAnvil::SQL::StatementProxy>  result;
    result.reset(new PrepareStatement(connection, statement));
#if 0
    if (type == ThorsAnvil::SQL::OneTime) {
        result.reset(new Statement(statement));
    }
    else if (type == Prepare) {
        result.reset(new PrepareStatement(statement));
    }
    else {
        throw std::runtime_error("Unknown Type for MySQL");
    }
#endif
    return result;
}

ThorsAnvil::SQL::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlConnection("mysql");

Connection::Connection(
                    std::string const& username,
                    std::string const& password,
                    std::string const& database,
                    std::map<std::string, std::string> const& options,
                    ConectReader& pr,
                    ConectWriter& pw)
    : packageReader(pr)
    , packageWriter(pw)
{
    using RespPackageHandShakeUPtr = std::unique_ptr<Detail::RespPackageHandShake>;
    RespPackageHandShakeUPtr    handshake = recvMessage<Detail::RespPackageHandShake>(
                                                {{0x0A, [](int firstByte, ConectReader& reader
                                                          )
                                                        {return new Detail::RespPackageHandShake(firstByte, reader);}
                                                 }
                                                });
    packageReader.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());
    packageWriter.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());

    Detail::RequPackageHandShakeResponse    handshakeresp(username, password, options, database, *handshake);
    std::unique_ptr<RespPackage>            ok = sendHandshakeMessage<RespPackage>(handshakeresp,
        {{0xFE, [](int firstByte, ConectReader& reader)
                {return new Detail::RespPackageAuthSwitchRequest(firstByte, reader);}
        }});

    if (!ok)
    {
        throw std::runtime_error("Connection::Connection: Handshake failed: Unexpected Package");
    }
    if (!(ok->isOK()))
    {
        std::cerr << "Runtime Error about to go\n";
        // Not currently we only support "mysql_native_password" authentication.
        // I welcome a pull request to support other authentication protocols.
        std::stringstream message;
        message << "Connection::Connection: Handshake failed: Got: " << (*ok);
        throw std::runtime_error(message.str());
    }
}

Connection::~Connection()
{
    // Shut Down
}

void Connection::removeCurrentPackage()
{
    packageReader.drop();
}

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "Connection.tpp"
#include "ConectReader.tpp"

template
std::unique_ptr<RespPackage> Connection::sendHandshakeMessage<RespPackage, Detail::RequPackageHandShakeResponse>
(Detail::RequPackageHandShakeResponse const&, ConectReader::OKMap const&);

template
std::unique_ptr<Detail::RespPackageHandShake> Connection::recvMessage<Detail::RespPackageHandShake>
(ConectReader::OKMap const&, bool);

template
std::unique_ptr<Detail::RespPackageHandShake> ConectReader::recvMessage<Detail::RespPackageHandShake>
(ConectReader::OKMap const&, bool);

#endif

