#include "ConectWriter.h"

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename Requ>
void Connection::sendMessageInternal(Requ const& request, bool resetWriter)
{
    packageReader.reset();
    if (resetWriter)
    {
        packageWriter.reset();
    }
    request.send(packageWriter);
}
template<typename Requ>
void Connection::sendMessage(Requ const& request)
{
    sendMessageInternal(request, true);
}
template<typename Requ>
std::unique_ptr<RespPackage> Connection::sendMessageGetResponse(Requ const& request, ConectReader::OKMap const& actions)
{
    sendMessageInternal(request, true);
    // FIXNOW (not need to use ResPackage below)
    return recvMessage(actions, false);
}
template<typename Resp, typename Requ>
std::unique_ptr<Resp> Connection::sendHandshakeMessage(Requ const& request, ConectReader::OKMap const& actions)
{
    sendMessageInternal(request, false);
    std::unique_ptr<RespPackage> result(recvMessage(actions, false));
    return downcastUniquePtr<Resp>(std::move(result));
}

    }
}
