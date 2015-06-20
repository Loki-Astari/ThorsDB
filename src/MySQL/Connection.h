
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
                   Options const& options,
                   PackageConReader& packageReader,
                   PackageConWriter& packageWriter);
        virtual ~Connection();

    private:
        template<typename V>    void unusedVariable(V const&){}

        template<typename Resp>
        std::unique_ptr<Resp> recvMessage(PackageConReader::ResponceType type, PackageConReader::PacketCompletion comp);
        template<typename Resp, typename Requ>
        std::unique_ptr<Resp> sendMessage(Requ const& request, PackageConReader::ResponceType type, PackageConReader::PacketCompletion comp);
};

template<typename Resp>
std::unique_ptr<Resp> Connection::recvMessage(PackageConReader::ResponceType type, PackageConReader::PacketCompletion comp)
{
    std::unique_ptr<PackageResp>    resp = packageReader.getNextPackage(type);
    if (resp->isError()) {
        throw resp;
    }

    if ((comp == PackageConReader::OK && resp->isOK()) || (comp == PackageConReader::EOF_OK && resp->isEOF())) {
        return nullptr;
    }

    // Throws exception if cast fails.
    Resp&                   resultRef = dynamic_cast<Resp&>(*resp);
    unusedVariable(resultRef);
    std::unique_ptr<Resp>   result(dynamic_cast<Resp*>(resp.release()));
    return result;
}

template<typename Resp, typename Requ>
std::unique_ptr<Resp> Connection::sendMessage(Requ const& request, PackageConReader::ResponceType type, PackageConReader::PacketCompletion comp)
{
    request.send(packageWriter);
    return recvMessage<Resp>(type, comp);
}
    }
}

#endif

