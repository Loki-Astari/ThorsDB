#ifndef THORS_ANVIL_POSTG_DEFAULT_MY_SQL_CONNECTION_H
#define THORS_ANVIL_POSTG_DEFAULT_MY_SQL_CONNECTION_H

#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "PackageBuffer.h"
#include "PostgresStream.h"
#include "ThorSQL/Connection.h"
#include <string>
#include <memory>

namespace ThorsAnvil
{
    namespace Postgres
    {

class DefaultPostGConnection: public ThorsAnvil::SQL::Lib::ConnectionProxy
{
    PostgresStream  stream;
    PackageBuffer   buffer;
    ConectReader    reader;
    ConectWriter    writer;
    Connection      connection;
    public:
        DefaultPostGConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               std::string const& database,
                               ThorsAnvil::SQL::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy> createStatementProxy(std::string const& statement) override;
};

    }
}

#endif
