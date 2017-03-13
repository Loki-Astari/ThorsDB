#ifndef THORS_ANVIL_SQL_CONNECTION_H
#define THORS_ANVIL_SQL_CONNECTION_H

#include "SQLUtil.h"
#include <string>
#include <map>
#include <memory>
#include <iostream>

namespace ThorsAnvil
{
    namespace SQL
    {

class Statement;
class StatementProxy;
class ConnectionProxy
{
    public:
        virtual ~ConnectionProxy()  = 0;
        virtual std::unique_ptr<StatementProxy> createStatementProxy(std::string const& statement, StatementType type) = 0;
};

inline ConnectionProxy::~ConnectionProxy() {}
using ConnectionCreator= std::function<std::unique_ptr<ConnectionProxy>(std::string const& host, int port,
                                                                        std::string const& username,
                                                                        std::string const& password,
                                                                        std::string const& database,
                                                                        Options const& options)>;
class Connection
{
    private:
        static std::map<std::string, ConnectionCreator>&    getCreators();

        friend class Statement;
        std::unique_ptr<StatementProxy> createStatementProxy(std::string const& statement, StatementType type);

        std::unique_ptr<ConnectionProxy>  proxy;
    public:
        Connection(std::string const& connection,
                      std::string const& username,
                      std::string const& password,
                      std::string const& database,
                      Options const& options = Options{});

        static void registerConnectionType(std::string const& schema, ConnectionCreator creator);
};

template<typename T>
class ConnectionCreatorRegister
{
    public:
        ConnectionCreatorRegister(std::string const& schema)
        {
            std::cerr << "Register: " << schema << "\n";
            Connection::registerConnectionType(schema, [](std::string const& host, int port,
                                                          std::string const& username,
                                                          std::string const& password,
                                                          std::string const& database,
                                                          Options const& options)
                    {
                        return std::unique_ptr<ConnectionProxy>(new T(host, port, username, password, database, options));
                    });
        }
};

    }
}

#endif
