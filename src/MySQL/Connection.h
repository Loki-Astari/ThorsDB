
#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ThorSQL/SQLUtil.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "RespPackage.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

using ThorsAnvil::SQL::Options;

class Connection
{
    ConectReader&   packageReader;
    ConectWriter&   packageWriter;
    public:
        Connection(std::string const& username,
                   std::string const& password,
                   std::string const& database,
                   Options const& options,
                   ConectReader& packageReader,
                   ConectWriter& packageWriter);
        virtual ~Connection();

        enum PacketContinuation { None, Reset};
        enum PacketCompletion   { OK,   EOF_OK };

        template<typename Resp>
        std::unique_ptr<Resp> recvMessage(PacketCompletion comp, ConectReader::OKAction actionOnOK);
        template<typename Resp, typename Requ>
        std::unique_ptr<Resp> sendMessage(Requ const& request, PacketContinuation cont, PacketCompletion comp, ConectReader::OKAction actionOnOK);
};

    }
}

#ifndef COVERAGE_MySQL
#include "Connection.tpp"
#endif

#endif

