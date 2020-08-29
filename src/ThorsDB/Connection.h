#ifndef THORS_ANVIL_DB_THORSDB_CONNECTION_H
#define THORS_ANVIL_DB_THORSDB_CONNECTION_H

/*
 * ThorsAnvil::DB::SQL::Connection  Represents a connection to a DB.
 *                                  It is used to initialize `Statement Objects`.
 *
 *                                  See: doc/usage.md for usage details
 *                                  See: doc/internal.md for implementation details
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
#include <functional>

namespace ThorsAnvil::DB::SQL
{
    namespace Lib
    {
        class StatementProxy;

        class ConnectionProxy
        {
            public:
                // constructor is called with host/port/username/password/database/options
                // See ConnectionCreator below.
                virtual ~ConnectionProxy()  = 0;
                // A function for creating DB specific instances of the StatementProxy objects.
                virtual std::unique_ptr<Lib::StatementProxy> createStatementProxy(std::string const& statement) = 0;

                virtual int getSocketId() const  = 0;
                virtual void setYield(std::function<void()>&&, std::function<void()>&&) = 0;
        };
        class YieldSetter
        {
            ConnectionProxy& connection;
            public:
                YieldSetter(ConnectionProxy& connection, std::function<void()>&& ry, std::function<void()>&& wy);
                ~YieldSetter();
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

        int getSocketId() const {return proxy->getSocketId();}
        void setYield(std::function<void()>&& yr, std::function<void()>&& yw){proxy->setYield(std::move(yr), std::move(yw));}
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

#endif
