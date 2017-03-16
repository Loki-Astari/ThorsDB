#include "Connection.h"
#include "ConectWriter.h"
#include "RespPackageHandShake.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RequPackageHandShakeResp.h"

using namespace ThorsAnvil::MySQL;

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
                                                        {return new RespPackageHandShake(firstByte, reader);}
                                                 }
                                                });
    std::unique_ptr<RespPackageHandShake> handshake = downcastUniquePtr<RespPackageHandShake>(std::move(initPack));
    packageReader.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());
    packageWriter.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());

    RequPackageHandShakeResponse    handshakeresp(username, password, options, database, *handshake);
    std::unique_ptr<RespPackage>    ok = sendHandshakeMessage<RespPackage>(handshakeresp,
        {{0xFE, [](int firstByte, ConectReader& reader)
                {return new RespPackageAuthSwitchRequest(firstByte, reader);}
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
