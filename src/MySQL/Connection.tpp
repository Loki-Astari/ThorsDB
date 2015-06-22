

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename Resp>
std::unique_ptr<Resp> Connection::recvMessage(PacketCompletion comp, PackageConReader::ResponceType type)
{
    std::unique_ptr<RespPackage>    resp = packageReader.getNextPackage(type);
    if (resp->isError()) {
        throw std::runtime_error(resp->message());;
    }

    if ((comp == OK && resp->isOK()) || (comp == EOF_OK && resp->isEOF())) {
        return nullptr;
    }

    // Throws exception if cast fails.
    Resp&                   resultRef = dynamic_cast<Resp&>(*resp);
    unusedVariable(resultRef);

    // Now we know the dynamic_cast will work and not return a nullptr.
    // release it do the dynamic cast and store it in a new unique_ptr
    // for return.
    std::unique_ptr<Resp>   result(dynamic_cast<Resp*>(resp.release()));
    return result;
}

template<typename Resp, typename Requ>
std::unique_ptr<Resp> Connection::sendMessage(Requ const& request, PacketContinuation cont, PacketCompletion comp, PackageConReader::ResponceType type)
{
    if (cont == Reset) {
        packageWriter.reset();
    }
    request.send(packageWriter);
    return recvMessage<Resp>(comp, type);
}

    }
}

