#include "Connection.h"
#include "Authentication.h"
#include "ConectWriter.h"
#include "RespPackageHandShake.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RequPackageHandShakeResp.h"
#include "RespPackageAuthMoreData.h"
#include "RequPackageSSLRequest.h"
#include "RequPackageAuthSwitchResp.h"
#include "ThorMySQL.h"

using namespace ThorsAnvil::DB::MySQL;

Connection::Connection(
                    std::string const& /*username*/,
                    std::string const& /*password*/,
                    std::string const& /*database*/,
                    Options const& /*options*/,
                    ConectReader& pr,
                    ConectWriter& pw)
    : packageReader(pr)
    , packageWriter(pw)
{
    // Don't use this class directly.
    // It requires a sub-sequent call to connectToServer before
    // it is fully initialized.

    // See ConnectionDefault (or ConnectionNonBlocking in ThorsNisse)
}

void Connection::conectToServer(std::string const& username,
                                std::string const& password,
                                std::string const& database,
                                Options const& options
                                )
{
    std::unique_ptr<RespPackage> serverResp;

    std::unique_ptr<RespPackage> initPack = recvMessage(
                                                {{0x0A, [](int firstByte, ConectReader& reader
                                                          )
                                                        {return new RespPackageHandShake(firstByte, reader);}
                                                 }
                                                });
    std::unique_ptr<RespPackageHandShake> handshake      = downcastUniquePtr<RespPackageHandShake>(std::move(initPack));
    std::unique_ptr<Authetication>        authentication = getAuthenticatonMethod(*this, handshake->getAuthPluginName(), options);
    std::string authResponse  = authentication->getAuthenticationString(username, password, database, handshake->getAuthPluginData());
    RequPackageHandShakeResponse    handshakeresp(username, password, options, database,
                                                  handshake->getCapabilities(),
                                                  authentication->getPluginName(),
                                                  authResponse);

    if (handshakeresp.getCapabilities() & CLIENT_SSL)
    {
        RequPackageSSLRequest           sslRequest(handshakeresp.getCapabilities());;
        sendMessage(sslRequest, false);
        establishSSLConnection();
    }


    initFromHandshake(handshakeresp.getCapabilities(), handshake->getCharset());
    serverResp =  sendMessageGetResponse(handshakeresp,
        false,
        {
            {0xFE, [](int firstByte, ConectReader& reader) -> RespPackage* {return new RespPackageAuthSwitchRequest(firstByte, reader);}},
            {0x01, [](int firstByte, ConectReader& reader) -> RespPackage* {return new RespPackageAuthMoreData(firstByte, reader);}}
        }
    );


    if (!serverResp)
    {
        throw std::domain_error("Connection::Connection: Handshake failed: Unexpected Package");
    }
    if (serverResp->is() == RespType::AuthSwitchRequest)
    {
        std::unique_ptr<RespPackageAuthSwitchRequest>   authSwitchRequest   = downcastUniquePtr<RespPackageAuthSwitchRequest>(std::move(serverResp));
        authentication  = getAuthenticatonMethod(*this, authSwitchRequest->getPluginName());
        std::string authResponse  = authentication->getAuthenticationString(username, password, database, handshake->getAuthPluginData());

        RequPackageAuthSwitchResponse   switchResp(username, password, options, database, authResponse);

        serverResp = sendMessageGetResponse(switchResp,
            false,
            {
                {0x01, [](int firstByte, ConectReader& reader) -> RespPackage* {return new RespPackageAuthMoreData(firstByte, reader);}}
            }
        );

        if (!serverResp)
        {
            throw std::domain_error("Connection::Connection: Auth Switch failed: Unexpected Package");
        }
    }

    if (serverResp->is() == RespType::Authentication)
    {
        serverResp = authentication->customAuthenticate(downcastUniquePtr<RespPackageAuthMoreData>(std::move(serverResp)), username, password, database, handshake->getAuthPluginData());

        if (!serverResp)
        {
            throw std::domain_error("Connection::Connection: Custom Auth failed: Unexpected Package");
        }
    }

    if (serverResp->isOK() == false)
    {
        throw std::domain_error(errorMsg("Connection::Connection: Handshake failed: Got: ", (*serverResp)));
    }
}

void Connection::initFromHandshake(unsigned long capabilities, unsigned long charset)
{
    packageReader.initFromHandshake(capabilities, charset);
    packageWriter.initFromHandshake(capabilities, charset);
}

void Connection::establishSSLConnection()
{
    packageWriter.establishSSLConnection();
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
std::unique_ptr<RespPackage> Connection::getResponse(ConectReader::OKMap const& actions)
{
    packageReader.reset();
    return recvMessage(actions);
}

void Connection::sendMessage(RequPackage const& request, bool startConv)
{
    if (!startConv)
    {
        packageWriter.simpleReset();
    }
    packageWriter.reset();
    request.send(packageWriter);
}
std::unique_ptr<RespPackage> Connection::sendMessageGetResponse(RequPackage const& request, bool startConv, ConectReader::OKMap const& actions)
{
    if (!startConv)
    {
        packageWriter.simpleReset();
    }
    packageWriter.reset();
    request.send(packageWriter);

    return getResponse(actions);
}
