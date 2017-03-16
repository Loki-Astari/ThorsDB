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
std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy>
DefaultMySQLConnection::createStatementProxy(std::string const& statement)
{
    std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy>  result;
    result.reset(new PrepareStatement(connection, statement));
    return result;
}

ThorsAnvil::SQL::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlConnection("mysql");

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
    std::unique_ptr<RespPackage> initPack = recvMessage(
                                                {{0x0A, [](int firstByte, ConectReader& reader
                                                          )
                                                        {return new Detail::RespPackageHandShake(firstByte, reader);}
                                                 }
                                                });
    std::unique_ptr<Detail::RespPackageHandShake> handshake = downcastUniquePtr<Detail::RespPackageHandShake>(std::move(initPack));
    packageReader.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());
    packageWriter.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());

    Detail::RequPackageHandShakeResponse    handshakeresp(username, password, options, database, *handshake);
    std::unique_ptr<RespPackage>            ok = sendHandshakeMessage<RespPackage>(handshakeresp,
        {{0xFE, [](int firstByte, ConectReader& reader)
                {return new Detail::RespPackageAuthSwitchRequest(firstByte, reader);}
         }
        });

    if (!ok)
    {
        throw std::domain_error("Connection::Connection: Handshake failed: Unexpected Package");
    }
    if (!(ok->isOK()))
    {
        throw std::domain_error(errorMsg("Connection::Connection: Handshake failed: Got: ", (*ok)));
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

std::unique_ptr<RespPackage> Connection::recvMessage(ConectReader::OKMap const& actions)
{
    std::unique_ptr<RespPackage>   result(packageReader.recvMessage(actions));
    return result;
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

#endif
