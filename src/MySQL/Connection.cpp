#include "Connection.h"
#include "ConectWriter.h"
#include "RespPackageHandShake.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RequPackageHandShakeResp.h"
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
    std::unique_ptr<RespPackageHandShake> handshake = downcastUniquePtr<RespPackageHandShake>(std::move(initPack));
    packageReader.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());
    packageWriter.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());

    RequPackageHandShakeResponse    handshakeresp(username, password, options, database, *handshake);
    std::unique_ptr<RespPackage>    serverResp = sendHandshakeMessage<RespPackage>(handshakeresp,
        {
         // The section below assumes only responses are OK/Error/RespPackageAuthSwitchRequest
         // If this changes to allow other responses then look at the section below were we
         // handle RespPackageAuthSwitchRequest
         {0xFE, [](int firstByte, ConectReader& reader)
                {return new RespPackageAuthSwitchRequest(firstByte, reader);}
         }
        });

    if (!serverResp)
    {
        throw std::domain_error("Connection::Connection: Handshake failed: Unexpected Package");
    }
    if (serverResp->isOK() == false && serverResp->isError() == false)
    {
        RequPackageAuthSwitchResponse   switchResp(username, password, options, database, *dynamic_cast<RespPackageAuthSwitchRequest*>(serverResp.get()));
        serverResp = sendHandshakeMessage<RespPackage>(switchResp, {});
        if (!serverResp)
        {
            throw std::domain_error("Connection::Connection: Auth Switch failed: Unexpected Package");
        }
    }
    if (serverResp->isOK() == false)
    {
        throw std::domain_error(errorMsg("Connection::Connection: Handshake failed: Got: ", (*serverResp)));
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
#include "ConectReader.h"
#include "RequPackagePrepare.h"
#include "RequPackagePrepareClose.h"
#include "RequPackagePrepareReset.h"
#include "RequPackagePrepareExecute.h"

template
std::unique_ptr<RespPackage> Connection::sendHandshakeMessage<RespPackage, RequPackageHandShakeResponse>
(RequPackageHandShakeResponse const&, ConectReader::OKMap const&);

template void Connection::sendMessage<RequPackagePrepareClose>(RequPackagePrepareClose const&);
template std::unique_ptr<RespPackage> Connection::sendMessageGetResponse<RequPackagePrepare>(RequPackagePrepare const&, std::map<int, std::function<RespPackage* (int, ConectReader&)>> const&);
template std::unique_ptr<RespPackage> Connection::sendMessageGetResponse<RequPackagePrepareReset>(RequPackagePrepareReset const&, std::map<int, std::function<RespPackage* (int, ConectReader&)>> const&);
template std::unique_ptr<RespPackage> Connection::sendMessageGetResponse<RequPackagePrepareExecute>(RequPackagePrepareExecute const&, std::map<int, std::function<RespPackage* (int, ConectReader&)>> const&);

#endif
