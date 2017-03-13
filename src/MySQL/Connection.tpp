

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename Resp>
std::unique_ptr<Resp> Connection::recvMessage(int expectedResult, ConectReader::OKAction expectedResultAction)
{
    return packageReader.recvMessage<Resp>(expectedResult, expectedResultAction);
}

template<typename Requ>
void Connection::sendMessage(Requ const& request, PacketContinuation cont)
{
    packageReader.reset();
    if (cont == Reset) {
        packageWriter.reset();
    }
    request.send(packageWriter);
}
template<typename Resp, typename Requ>
std::unique_ptr<Resp> Connection::sendMessage(Requ const& request, PacketContinuation cont, int expectedResult, ConectReader::OKAction expectedResultAction)
{
    sendMessage(request, cont);
    return recvMessage<Resp>(expectedResult, expectedResultAction);
}

    }
}

