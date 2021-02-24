#ifndef THORS_ANVIL_DB_MONGO_MONGO_CONNECTION_H
#define THORS_ANVIL_DB_MONGO_MONGO_CONNECTION_H

#include "MongoBuffer.h"
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
    using CursorInfo    = std::pair<std::int32_t, std::int32_t>;
    using StreamBuffer  = ThorsIO::SocketStreamBuffer;
    using ConnectSocket = ThorsIO::ConnectSocket;
    using IOSocketStream= ThorsIO::IOSocketStream<MongoBuffer>;

    private:
        using CursorMap   = std::map<CursorId, CursorInfo>;

        ConnectSocket                           socket;
        IOSocketStream                          stream;
        std::unique_ptr<MongoBuffer>            buffer;
        std::string                             dbName;
        std::int8_t                             compression;
        CursorMap                               openCursors;
        CursorId                                lastCursor = -1;
    public:
        MongoConnection(std::string const& host, int port,
                        std::string const& username,
                        std::string const& password,
                        std::string const& database,
                        ThorsAnvil::DB::Access::Options const& options);
        ~MongoConnection();
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
