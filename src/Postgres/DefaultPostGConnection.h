#ifndef THORS_ANVIL_DB_POSTGRESS_DEFAULT_MY_SQL_CONNECTION_H
#define THORS_ANVIL_DB_POSTGRESS_DEFAULT_MY_SQL_CONNECTION_H

#include "ThorSQL/Connection.h"
#include <string>
#include <memory>

namespace ThorsAnvil::DB::Postgres
{

class DefaultPostGConnection: public ThorsAnvil::DB::SQL::Lib::ConnectionProxy
{
    public:
        DefaultPostGConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               std::string const& database,
                               ThorsAnvil::DB::SQL::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::DB::SQL::Lib::StatementProxy> createStatementProxy(std::string const& statement) override;
        virtual int getSocketId() const override {throw std::runtime_error("TODO");}
        virtual void setYield(std::function<void()>&&, std::function<void()>&&) override {throw std::runtime_error("TODO");}
};

}

#endif
