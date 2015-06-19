
#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ThorSQL/Connection.h"
#include "PackageReader.h"
#include "PackageBuffer.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

class ConnectionStream
{
};

class Connection
{
    PackageReader&  packageReader;
    public:
        Connection(std::string const& username,
                   std::string const& password,
                   std::map<std::string, std::string> const& options,
                   PackageReader& packageReader);
        virtual ~Connection();
};

    }
}

#endif

