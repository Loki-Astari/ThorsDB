
#ifndef THORS_ANVIL_MYSQL_CONNECTION_H
#define THORS_ANVIL_MYSQL_CONNECTION_H

#include "ThorSQL/SQLUtil.h"
#include "PackageConReader.h"
#include <string>
#include <map>

namespace ThorsAnvil
{
    namespace MySQL
    {

using ThorsAnvil::SQL::Options;

class Connection
{
    PackageConReader&  packageReader;
    public:
        Connection(std::string const& username,
                   std::string const& password,
                   Options const& options,
                   PackageConReader& packageReader);
        virtual ~Connection();
};

    }
}

#endif

