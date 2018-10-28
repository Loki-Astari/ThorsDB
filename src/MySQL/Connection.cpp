#include "Connection.h"
#include "Authentication.h"
#include "ConectWriter.h"
#include "RespPackageHandShake.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RequPackageHandShakeResp.h"
#include "RespPackageAuthMoreData.h"
#include "RequPackageAuthSwitchResp.h"

using namespace ThorsAnvil::MySQL;

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
    std::unique_ptr<RespPackage> initPack = recvMessage(
                                                {{0x0A, [](int firstByte, ConectReader& reader
                                                          )
                                                        {return new RespPackageHandShake(firstByte, reader);}
                                                 }
                                                });
    std::unique_ptr<RespPackageHandShake> handshake      = downcastUniquePtr<RespPackageHandShake>(std::move(initPack));
    std::unique_ptr<Authetication>        authentication = getAuthenticatonMethod(*this, handshake->getAuthPluginName(), options);
    std::unique_ptr<RespPackage>          serverResp     = authentication->sendHandShakeResponse(username, password, database, handshake->getAuthPluginData(), handshake->getCapabilities(), handshake->getCharset());

    if (!serverResp)
    {
        throw std::domain_error("Connection::Connection: Handshake failed: Unexpected Package");
    }
    if (serverResp->is() == RespType::AuthSwitchRequest)
    {
        std::unique_ptr<RespPackageAuthSwitchRequest>   authSwitchRequest   = downcastUniquePtr<RespPackageAuthSwitchRequest>(std::move(serverResp));
        authentication  = getAuthenticatonMethod(*this, authSwitchRequest->getPluginName());
        serverResp      = authentication->sendSwitchResponse(username, password, database, handshake->getAuthPluginData());

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

    packageReader.reset();
    return recvMessage(actions);
}
