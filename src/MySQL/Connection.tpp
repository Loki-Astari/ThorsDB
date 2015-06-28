

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename Resp>
std::unique_ptr<Resp> Connection::recvMessage(int expectedResult, ConectReader::OKAction expectedResultAction)
{
    std::unique_ptr<RespPackage>    resp = packageReader.getNextPackage(expectedResult, expectedResultAction);
    if (resp->isError()) {
        throw std::runtime_error(resp->message());;
    }

    // Throws exception if cast fails.
    Resp&                   resultRef = dynamic_cast<Resp&>(*resp);
    [&resultRef](){}();

    // Now we know the dynamic_cast will work and not return a nullptr.
    // release it do the dynamic cast and store it in a new unique_ptr
    // for return.
    std::unique_ptr<Resp>   result(dynamic_cast<Resp*>(resp.release()));
    return result;
}

template<typename Resp, typename Requ>
std::unique_ptr<Resp> Connection::sendMessage(Requ const& request, PacketContinuation cont, int expectedResult, ConectReader::OKAction expectedResultAction)
{
    packageReader.reset();
    if (cont == Reset) {
        packageWriter.reset();
    }
    request.send(packageWriter);
    return recvMessage<Resp>(expectedResult, expectedResultAction);
}

    }
}

