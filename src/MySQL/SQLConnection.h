
#ifndef THORS_ANVIL_SQL_SQLCONNECTION_H
#define THORS_ANVIL_SQL_SQLCONNECTION_H

#include <string>
#include <map>
#include <memory>

namespace ThorsAnvil
{
    namespace SQL
    {

class SQLConnectionProxy
{
};
class SQLConnection
{
    private:
        std::unique_ptr<SQLConnectionProxy>  proxy;
    public:
        SQLConnection(std::string const& connection,
                      std::string const& username,
                      std::string const& password,
                      std::map<std::string, std::string> const& options = std::map<std::string, std::string>());
};
class SQLStatement
{
    public:
        virtual ~SQLStatement()    = 0;
};

    }
}

#endif


