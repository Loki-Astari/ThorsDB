#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ThorSQL/Connection.h"
#include "ThorSQL/SQLUtil.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "PackageBuffer.h"
#include "PackageStream.h"
#include <string>

namespace ThorsAnvil
{
    namespace MySQL
    {

using ThorsAnvil::SQL::Options;

class ConectWriter;
class RespPackage;
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

        template<typename Resp>
        std::unique_ptr<Resp> recvMessage(ConectReader::OKMap const& actions, bool expectedEOF = false);
        template<typename Resp, typename Requ>
        std::unique_ptr<Resp> sendHandshakeMessage(Requ const& hs, ConectReader::OKMap const& actions);
        template<typename Resp, typename Requ>
        std::unique_ptr<Resp> sendMessage(Requ const& request, ConectReader::OKMap const&    actions     = {});
        template<typename Requ>
        void                  sendMessage(Requ const& request);
        void                  removeCurrentPackage();
    private:
        template<typename Requ>
        void sendMessageInternal(Requ const& request, bool resetWriter);
};

class DefaultMySQLConnection: public ThorsAnvil::SQL::ConnectionProxy
{
    private:
        MySQLStream                                 stream;
        PackageBufferMySQLDebugBuffer<MySQLStream>  buffer;
        ConectReader                                reader;
        ConectWriter                                writer;
        Connection                                  connection;
    public:
        DefaultMySQLConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               std::string const& database,
                               ThorsAnvil::SQL::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::SQL::StatementProxy> createStatementProxy(
                                                                        std::string const& statement,
                                                                        ThorsAnvil::SQL::StatementType /*type*/
                                                                    ) override;
};

    }
}

#ifndef COVERAGE_MySQL
#include "Connection.tpp"
#endif

#endif
