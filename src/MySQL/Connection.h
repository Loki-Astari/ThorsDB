
#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ThorSQL/SQLUtil.h"
#include "PackageConReader.h"
#include "PackageConWriter.h"
#include "PackageResp.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

using ThorsAnvil::SQL::Options;

class Connection
{
    PackageConReader&   packageReader;
    PackageConWriter&   packageWriter;
    public:
        Connection(std::string const& username,
                   std::string const& password,
                   std::string const& database,
                   Options const& options,
                   PackageConReader& packageReader,
                   PackageConWriter& packageWriter);
        virtual ~Connection();

    private:
        enum PacketContinuation { None, Reset};
        enum PacketCompletion   { OK,   EOF_OK };

        template<typename V>    void unusedVariable(V const&){}

        template<typename Resp>
        std::unique_ptr<Resp> recvMessage(PacketCompletion comp, PackageConReader::ResponceType type);
        template<typename Resp, typename Requ>
        std::unique_ptr<Resp> sendMessage(Requ const& request, PacketContinuation cont, PacketCompletion comp, PackageConReader::ResponceType type);
};

template<typename Resp>
std::unique_ptr<Resp> Connection::recvMessage(PacketCompletion comp, PackageConReader::ResponceType type)
{
    std::unique_ptr<PackageResp>    resp = packageReader.getNextPackage(type);
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

#endif

