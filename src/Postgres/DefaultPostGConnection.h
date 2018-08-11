#ifndef THORS_ANVIL_POSTG_DEFAULT_MY_SQL_CONNECTION_H
#define THORS_ANVIL_POSTG_DEFAULT_MY_SQL_CONNECTION_H

#include "ThorSQL/Connection.h"
#include <string>
#include <memory>

namespace ThorsAnvil
{
    namespace Postgres
    {

class DefaultPostGConnection: public ThorsAnvil::SQL::Lib::ConnectionProxy
{
    public:
        DefaultPostGConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               std::string const& database,
                               ThorsAnvil::SQL::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy> createStatementProxy(std::string const& statement) override;
        virtual int getSocketId() const override {throw std::runtime_error("TODO");}
        virtual void setYield(std::function<void()>&&, std::function<void()>&&) override {throw std::runtime_error("TODO");}
};

    }
}

#endif
