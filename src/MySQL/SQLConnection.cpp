
#include "SQLConnection.h"
#include "MySQLConnection.h"
#include <cerrno>
#include <cstdlib>

using namespace ThorsAnvil::SQL;

SQLConnection::SQLConnection(std::string const& connection,
                             std::string const& username,
                             std::string const& password,
                             std::map<std::string, std::string> const& options)
{
    std::size_t     schemaEnd   = connection.find(':');
    if (schemaEnd == std::string::npos || connection[schemaEnd + 1] != '/' || connection[schemaEnd + 2] != '/') {
        throw std::runtime_error("SQLConnection::SQLConnection: Failed to find schema: " + connection);
    }

    bool        hasPort     = true;
    std::size_t hostEnd     = connection.find(':', schemaEnd + 3);

    if (hostEnd == std::string::npos) {
        hasPort = false;
        hostEnd = connection.size();
    }

    std::string schema      = connection.substr(0, schemaEnd);
    std::string host        = connection.substr(schemaEnd + 3, hostEnd - schemaEnd - 3);
    std::string port        = hasPort ? connection.substr(hostEnd + 1) : "0";

    errno                   = 0;
    int         portNumber  = std::strtol(port.c_str(), nullptr, 10);

    if (schema != "mysql" || host == "" || errno != 0) {
        throw std::runtime_error("SQLConnection::SQLConnection: Failed to parse connection: " + connection);
    }

    proxy.reset(new MySQL::MySQLConnection(host, portNumber, username, password, options));
}

