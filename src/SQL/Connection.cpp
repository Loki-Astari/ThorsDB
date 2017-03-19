#include "Connection.h"
#include "Statement.h"
#include <cerrno>
#include <cstdlib>

using namespace ThorsAnvil::SQL;

Connection::Connection(std::string const& connection,
                       std::string const& username,
                       std::string const& password,
                       std::string const& database,
                       Options const& options)
{
    std::cout << "Connecting: " << connection << "\n";
    // Parse a connection URI.
    std::size_t     schemaEnd   = connection.find(':');
    if (schemaEnd == std::string::npos || connection[schemaEnd + 1] != '/' || connection[schemaEnd + 2] != '/')
    {
        throw std::logic_error(
                errorMsg("ThorsAnvil::SQL::Connection::Connection: ",
                         "Failed to find schema: ",
                         connection,
                         "\n Expected: <schema>:://<host>[:<port>]"
              ));
    }

    bool        hasPort     = true;
    std::size_t hostEnd     = connection.find(':', schemaEnd + 3);

    if (hostEnd == std::string::npos)
    {
        hasPort = false;
        hostEnd = connection.size();
    }

    std::string schema      = connection.substr(0, schemaEnd);
    std::string host        = connection.substr(schemaEnd + 3, hostEnd - schemaEnd - 3);
    std::string port        = hasPort ? connection.substr(hostEnd + 1) : "0";
    std::cout << "Connecting: Schema -> " << schema << "\n";

    errno                   = 0;
    char*       endPtr;
    int         portNumber  = std::strtol(port.c_str(), &endPtr, 10);
    auto        creator     = getCreators().find(schema);

    if (host == "" || errno != 0 || *endPtr != '\0')
    {
        throw std::logic_error(
                errorMsg("ThorsAnvil::SQL::Connection::Connection: ",
                         "Failed to parse connection: ",
                         connection,
                         "\n Expected: <schema>:://<host>[:<port>]"
              ));
    }

    // Use the schema is used to pull a registered creator object.
    if (creator == getCreators().end())
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::SQL::Connection::Conection: ",
                         "Schema for unregister DB type: ",
                         schema, " From: ", connection
              ));
    }
    // Finally use the creator object to construct a ConnectionProxy.
    proxy   = creator->second(host, portNumber, username, password, database, options);
}

std::map<std::string, Lib::ConnectionCreator>& Connection::getCreators()
{
    static std::map<std::string, Lib::ConnectionCreator> creators;
    return creators;
}
void Connection::registerConnectionType(std::string const& schema, Lib::ConnectionCreator creator)
{
    std::cout << "Register: " << schema << "\n\n";
    getCreators().emplace(schema, creator);
}


std::unique_ptr<Lib::StatementProxy> Connection::createStatementProxy(std::string const& statement)
{
    return proxy->createStatementProxy(statement);
}

Lib::ConnectionProxy::~ConnectionProxy()
{}
