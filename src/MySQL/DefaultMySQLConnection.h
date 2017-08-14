#ifndef THORS_ANVIL_MYSQL_DEFAULT_MY_SQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_DEFAULT_MY_SQL_CONNECTION_H

#include "MySQLStream.h"
#include "PackageBuffer.h"
#include "ConnectionDefault.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "ThorSQL/Connection.h"
#include <string>
#include <memory>

namespace ThorsAnvil
{
    namespace MySQL
    {

class DefaultMySQLConnection: public ThorsAnvil::SQL::Lib::ConnectionProxy
{
    private:
        MySQLStream                 stream;
        PackageBuffer<MySQLStream>  buffer;
        ConectReader                reader;
        ConectWriter                writer;
        ConnectionDefault           connection;
    public:
        DefaultMySQLConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               std::string const& database,
                               ThorsAnvil::SQL::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy> createStatementProxy(std::string const& statement) override;
};

    }
}

#endif
