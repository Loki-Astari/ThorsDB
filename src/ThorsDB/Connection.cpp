#include "Connection.h"
#include "Statement.h"
#include "ThorsIOUtil/Utility.h"
#include <cerrno>
#include <cstdlib>

using namespace ThorsAnvil::DB::Access;
using ThorsAnvil::Utility::buildErrorMessage;

Connection::Connection(std::string const& connection,
                       std::string const& username,
                       std::string const& password,
                       std::string const& database,
                       Options const& options)
{
    // Parse a connection URI.
    std::size_t     schemaEnd   = connection.find(':');
    if (schemaEnd == std::string::npos || connection[schemaEnd + 1] != '/' || connection[schemaEnd + 2] != '/')
    {
        ThorsLogAndThrowLogical("ThorsAnvil::DB::Access::Connection",
                                "Connection",
                                "Failed to find schema: ", connection, "\n Expected: <schema>:://<host>[:<port>]");
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

    errno                   = 0;
    char*       endPtr;
    int         portNumber  = std::strtol(port.c_str(), &endPtr, 10);
    auto        creator     = getCreators().find(schema);

    if (host == "" || errno != 0 || *endPtr != '\0')
    {
        ThorsLogAndThrowLogical("ThorsAnvil::DB::Access::Connection",
                                "Connection",
                                "Failed to parse connection: ", connection, "\n Expected: <schema>:://<host>[:<port>]");
    }

    // Use the schema is used to pull a registered creator object.
    if (creator == getCreators().end())
    {
        ThorsLogAndThrow("ThorsAnvil::DB::Access::Connection",
                         "Conection",
                         "Schema for unregister DB type: ", schema, " From: ", connection);
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
    getCreators().emplace(schema, creator);
}


std::unique_ptr<Lib::StatementProxy> Connection::createStatementProxy(std::string const& statement)
{
    return proxy->createStatementProxy(statement);
}

Lib::ConnectionProxy::~ConnectionProxy()
{}

Lib::YieldSetter::YieldSetter(Lib::ConnectionProxy& connection, std::function<void()>&& ry, std::function<void()>&& wy)
    : connection(connection)
{
    connection.setYield(std::move(ry), std::move(wy));
}
Lib::YieldSetter::~YieldSetter()
{
    connection.setYield([](){}, [](){});
}
