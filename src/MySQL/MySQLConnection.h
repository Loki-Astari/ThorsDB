
#ifndef THORS_ANVIL_MYSQL_MYSQLCONNECTION_H
#define THORS_ANVIL_MYSQL_MYSQLCONNECTION_H

#include "ThorSQL/Connection.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

class MySQLConnection: public SQL::ConnectionProxy
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

