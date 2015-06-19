
#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ThorSQL/SQLUtil.h"
#include "PackageReader.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

using ThorsAnvil::SQL::Options;

class Connection
{
    PackageReader&  packageReader;
    public:
        Connection(std::string const& username,
                   std::string const& password,
                   Options const& options,
                   PackageReader& packageReader);
        virtual ~Connection();
};

    }
}

#endif

