#ifndef THORS_ANVIL_DB_MONGO_DEFAULT_MY_SQL_CONNECTION_H
#define THORS_ANVIL_DB_MONGO_DEFAULT_MY_SQL_CONNECTION_H

#include "ThorsSocket/Socket.h"
#include "ThorsSocket/SocketStream.h"
#include "ThorsDB/Connection.h"
#include <string>
#include <memory>

namespace ThorsAnvil::DB::Mongo
{

class MongoConnection: public ThorsAnvil::DB::Access::Lib::ConnectionProxy
{
    private:
        ThorsAnvil::ThorsIO::ConnectSocket      socket;
        ThorsAnvil::ThorsIO::IOSocketStream     stream;
    public:
        MongoConnection(std::string const& host, int port,
                        std::string const& username,
                        std::string const& password,
                        std::string const& database,
                        ThorsAnvil::DB::Access::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::DB::Access::Lib::StatementProxy> createStatementProxy(std::string const& statement) override;

        virtual int getSocketId() const override;
        virtual void setYield(std::function<void()>&&, std::function<void()>&&) override;
};

}

#endif
