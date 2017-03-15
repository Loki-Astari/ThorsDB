#ifndef THORS_ANVIL_SQL_CONNECTION_H
#define THORS_ANVIL_SQL_CONNECTION_H

/*
 * ThorsAnvil::SQL::Connection      Represents a connection to a DB.
 *                                  It is used to initialize `Statement Objects`.
 *
 *                                  It is a simple PIMPLE object were the actual
 *                                  functionality is provided by a ConnectionProxy object.
 *
 *                                  The Connection class also acts as a factory class for
 *                                  creating ConnectionProxy objects. Each library that
 *                                  defines an implementation should also register a
 *                                  ConnectionCreator object with the Connector so that it
 *                                  can automatically create the ConnectionProxy from the URI.
 *
 *                                  The class ConnectionCreatorRegister is useful as a simple
 *                                  way to register ConnectionCreator objects. Each library
 *                                  should declare a single object of this type in the global
 *                                  scope. Its instantiation will automatically register the
 *                                  creator when the shared library is loaded.
 *
 * Example:
 *      Options    conectOptions;
 *      Connextion conect("mysql://<host>[:<port]", "<username>", "<password>", "<DB Name>", conectOptions);
 *
 *
 * Other Classes:
 * ==============
 *      Lib::ConnectionProxy:                 Used by libraries that implement connection class.
 *      Lib::ConnectionCreator:               Used by libraries that implement connection class.
 *      Lib::ConnectionCreatorRegister:       Used by libraries that implement connection class.
 */

#include "SQLUtil.h"
#include <string>
#include <map>
#include <memory>
#include <iostream>

namespace ThorsAnvil
{
    namespace SQL
    {
        namespace Lib
        {
            class StatementProxy;

            class ConnectionProxy
            {
                public:
                    virtual ~ConnectionProxy()  = 0;
                    virtual std::unique_ptr<Lib::StatementProxy> createStatementProxy(std::string const& statement) = 0;
            };
            template<typename T>
            class ConnectionCreatorRegister
            {
                public:
                    ConnectionCreatorRegister(std::string const& schema);
            };

                using ConnectionCreator= std::function<std::unique_ptr<Lib::ConnectionProxy>(std::string const& host, int port,
                                                                                             std::string const& username,
                                                                                             std::string const& password,
                                                                                             std::string const& database,
                                                                                             Options const& options)>;
        }

class Statement;
class Connection
{
    private:
        static std::map<std::string, Lib::ConnectionCreator>&    getCreators();

        friend class Statement;
        std::unique_ptr<Lib::StatementProxy> createStatementProxy(std::string const& statement);

        std::unique_ptr<Lib::ConnectionProxy>  proxy;
    public:
        Connection(std::string const& connection,
                      std::string const& username,
                      std::string const& password,
                      std::string const& database,
                      Options const& options = Options{});

        static void registerConnectionType(std::string const& schema, Lib::ConnectionCreator creator);
};

template<typename T>
Lib::ConnectionCreatorRegister<T>::ConnectionCreatorRegister(std::string const& schema)
{
    std::cerr << "Register: " << schema << "\n";
    Connection::registerConnectionType(schema, [](std::string const& host, int port,
                                                  std::string const& username,
                                                  std::string const& password,
                                                  std::string const& database,
                                                  Options const& options)
            {
                return std::unique_ptr<Lib::ConnectionProxy>(new T(host, port, username, password, database, options));
            });
}

    }
}

#endif
