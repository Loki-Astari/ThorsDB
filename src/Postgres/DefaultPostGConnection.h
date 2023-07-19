#ifndef THORS_ANVIL_DB_POSTGRESS_DEFAULT_MY_SQL_CONNECTION_H
#define THORS_ANVIL_DB_POSTGRESS_DEFAULT_MY_SQL_CONNECTION_H

#include "PostgresConfig.h"
#include "ThorsDB/Connection.h"
#include "ThorsLogging/ThorsLogging.h"
#include <string>
#include <memory>

namespace ThorsAnvil::DB::Postgres
{

class DefaultPostGConnection: public ThorsAnvil::DB::Access::Lib::ConnectionProxy
{
    public:
        DefaultPostGConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               std::string const& database,
                               ThorsAnvil::DB::Access::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::DB::Access::Lib::StatementProxy> createStatementProxy(std::string const& statement) override;
        virtual int getSocketId() const override
        {
            ThorsLogAndThrowFatal("ThorsAnvil::DB::Postgres::DefaultPostGConnection", "getSocketId", "TODO");
        }
        virtual void setYield(std::function<void()>&&, std::function<void()>&&) override
        {
            ThorsLogAndThrowFatal("ThorsAnvil::DB::Postgres::DefaultPostGConnection", "setYield", "TODO");
        }
};

}

#endif
