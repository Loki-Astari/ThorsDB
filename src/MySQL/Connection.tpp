#include "ConectWriter.h"
#include "RespPackage.h"

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename Requ>
void Connection::sendMessage(Requ const& request)
{
    packageWriter.reset();
    request.send(packageWriter);
}
template<typename Requ>
std::unique_ptr<RespPackage> Connection::sendMessageGetResponse(Requ const& request, ConectReader::OKMap const& actions)
{
    packageWriter.reset();
    request.send(packageWriter);

    packageReader.reset();
    return recvMessage(actions);
}
template<typename Resp, typename Requ>
std::unique_ptr<Resp> Connection::sendHandshakeMessage(Requ const& request, ConectReader::OKMap const& actions)
{
    packageWriter.simpleReset();
    std::unique_ptr<RespPackage> result = sendMessageGetResponse<Requ>(request, actions);
    return downcastUniquePtr<Resp>(std::move(result));
}

    }
}
