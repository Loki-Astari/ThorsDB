#ifndef THORS_ANVIL_DB_MONGO_MONGO_CONNECTION_H
#define THORS_ANVIL_DB_MONGO_MONGO_CONNECTION_H

#include "Op_Reply.h"
#include "ThorsSocket/Socket.h"
#include "ThorsSocket/SocketStream.h"
#include "ThorsDB/Connection.h"
#include "ThorsDB/Statement.h"
#include <string>
#include <memory>

namespace ThorsAnvil::DB::Mongo
{

class MongoConnection: public ThorsAnvil::DB::Access::Lib::ConnectionProxy
{
    using CursorInfo  = std::pair<std::int32_t, std::int32_t>;

    private:
        using CursorMap   = std::map<CursorId, CursorInfo>;

        ThorsAnvil::ThorsIO::ConnectSocket      socket;
        ThorsAnvil::ThorsIO::IOSocketStream     stream;
        std::string                             dbName;
        CursorMap                               openCursors;
        CursorId                                lastCursor = -1;
    public:
        MongoConnection(std::string const& host, int port,
                        std::string const& username,
                        std::string const& password,
                        std::string const& database,
                        ThorsAnvil::DB::Access::Options const& options);
        virtual std::unique_ptr<ThorsAnvil::DB::Access::Lib::StatementProxy> createStatementProxy(std::string const& statement) override;

        virtual int getSocketId() const override;
        virtual void setYield(std::function<void()>&&, std::function<void()>&&) override;
        CursorId                getLastOpenCursor(bool close = false);
        std::vector<CursorId>   getAllOpenCursors(bool close = false);
        CursorInfo              getCursorInfo(CursorId cursor) const;

        template<typename T>
        MongoConnection& operator<<(T&& value);
        template<typename T>
        MongoConnection& operator>>(T&& value);
        explicit operator bool();
};

}

#include "MongoConnection.tpp"

#endif
