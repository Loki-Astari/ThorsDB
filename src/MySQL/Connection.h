
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
    public:
        ConectReader&   packageReader;
        ConectWriter&   packageWriter;

        Connection(std::string const& username,
                   std::string const& password,
                   std::string const& database,
                   Options const& options,
                   ConectReader& packageReader,
                   ConectWriter& packageWriter);
        virtual ~Connection();

        enum PacketContinuation { None, Reset};

        template<typename Resp>
        std::unique_ptr<Resp> recvMessage(int expectedResult, ConectReader::OKAction expectedResultAction);
        template<typename Resp, typename Requ>
        std::unique_ptr<Resp> sendMessage(Requ const& request, PacketContinuation cont, int expectedResult, ConectReader::OKAction expectedResultAction);
        template<typename Requ>
        void                  sendMessage(Requ const& request, PacketContinuation cont);
};

    }
}

#ifndef COVERAGE_MySQL
#include "Connection.tpp"
#endif

#endif

