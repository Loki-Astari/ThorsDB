
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

    }
}

#ifndef COVERAGE_TEST
#include "Connection.tpp"
#endif

#endif

