
#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ThorSQL/Connection.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

class Connection: public SQL::ConnectionProxy
{
    public:
        Connection(std::string const& host, int port,
                   std::string const& username,
                   std::string const& password,
                   std::map<std::string, std::string> const& options);
};

    }
}

#endif

