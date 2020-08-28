#ifndef THORS_ANVIL_DB_MYSQL_CONNECTION_DEFAULT_H
#define THORS_ANVIL_DB_MYSQL_CONNECTION_DEFAULT_H

#include "Connection.h"

namespace ThorsAnvil::DB::MySQL
{

class ConnectionDefault: public Connection
{
    public:
        ConnectionDefault(std::string const& username,
                          std::string const& password,
                          std::string const& database,
                          Options const& options,
                          ConectReader& packageReader,
                          ConectWriter& packageWriter)
            : Connection(username, password, database, options, packageReader, packageWriter)
        {
            conectToServer(username, password, database, options);
        }
};

}

#endif
