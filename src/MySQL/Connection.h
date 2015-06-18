
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

class Connection: public SQL::ConnectionProxy
{
    static PackageReader<PackageBufferMySQLDebugBuffer>   defaultPackageReader;

    int                 socket;
    PackageReaderBase&  packageReader;
    public:
        Connection(std::string const& host, int port,
                   std::string const& username,
                   std::string const& password,
                   std::map<std::string, std::string> const& options,
                   PackageReaderBase& packageReader = defaultPackageReader);
        virtual ~Connection();

    private:
        static int connect(std::string const& host, int port);
};

    }
}

#endif

