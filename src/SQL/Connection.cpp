
#include "Connection.h"
#include <cerrno>
#include <cstdlib>

using namespace ThorsAnvil::SQL;

Connection::Connection(std::string const& connection,
                       std::string const& username,
                       std::string const& password,
                       Options const& options)
{
    std::size_t     schemaEnd   = connection.find(':');
    if (schemaEnd == std::string::npos || connection[schemaEnd + 1] != '/' || connection[schemaEnd + 2] != '/') {
        throw std::runtime_error("Connection::Connection: Failed to find schema: " + connection);
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
    char*       endPtr;
    int         portNumber  = std::strtol(port.c_str(), &endPtr, 10);
    auto        creator     = getCreators().find(schema);

    if (host == "" || errno != 0 || *endPtr != '\0') {
        throw std::runtime_error("Connection::Connection: Failed to parse connection: " + connection);
    }

    if (creator == getCreators().end()) {
        throw std::runtime_error("Connection::Conection: Schema for unregister DB type: " + schema + " From: " + connection);
    }
    proxy   = creator->second(host, portNumber, username, password, options);
}

std::map<std::string, ConnectionCreator>& Connection::getCreators()
{
    static std::map<std::string, ConnectionCreator> creators;
    return creators;
}
void Connection::registerConnectionType(std::string const& schema, ConnectionCreator creator)
{
    getCreators().emplace(schema, creator);
}

