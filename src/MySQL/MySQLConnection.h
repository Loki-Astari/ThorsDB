
#ifndef THORS_ANVIL_MYSQL_MYSQLCONNECTION_H
#define THORS_ANVIL_MYSQL_MYSQLCONNECTION_H

#include "SQLConnection.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

class MySQLConnection: public SQL::SQLConnectionProxy
{
    public:
        MySQLConnection(std::string const& host, int port,
                        std::string const& username,
                        std::string const& password,
                        std::map<std::string, std::string> const& options);
};

    }
}

#endif

