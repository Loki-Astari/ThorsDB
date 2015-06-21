
#ifndef THORS_ANVIL_SQL_CONNECTION_H
#define THORS_ANVIL_SQL_CONNECTION_H

#include "SQLUtil.h"
#include <string>
#include <map>
#include <memory>

namespace ThorsAnvil
{
    namespace SQL
    {

class ConnectionProxy
{
    public:
        virtual ~ConnectionProxy()  = 0;
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
            Connection::registerConnectionType(schema, [](std::string const& host, int port,
                                                          std::string const& username,
                                                          std::string const& password,
                                                          std::string const& database,
                                                          Options const& options)
                    {
                        return std::unique_ptr<ConnectionProxy>(new T(host, port , username, password, database, options));
                    });
        }
};
class Statement
{
    public:
        virtual ~Statement()    = 0;
};

    }
}

#endif


