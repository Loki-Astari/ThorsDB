#ifndef THORSANVIL_DB_MONGO_OP_QUERY_H
#define THORSANVIL_DB_MONGO_OP_QUERY_H

#include "Op.h"
#include "MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>
#include <map>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-query
namespace ThorsAnvil::DB::Mongo
{

enum class OP_QueryFlag : std::int32_t
{
    empty           = 0,
                                            // Bit 0 is reserved. Must be set to 0.
    TailableCursor  = 1 << 1,               // TailableCursor means cursor is not closed when the last data is retrieved.
                                            // Rather, the cursor marks the final object’s position.
                                            // You can resume using the cursor later, from where it was located, if more data were received.
                                            // Like any “latent cursor”, the cursor may become invalid at some point (CursorNotFound)
    SlaveOk         = 1 << 2,               // Allow query of replica slave. Normally these return an error except for namespace “local”.
    OplogReplay     = 1 << 3,               // Starting in MongoDB 4.4, you need not specify this flag because the optimization automatically
                                            // happens for eligible queries on the oplog. See oplogReplay for more information.
    NoCursorTimeout = 1 << 4,               // The server normally times out idle cursors after an inactivity period (10 minutes)
                                            // to prevent excess memory use. Set this option to prevent that.
    AwaitData       = 1 << 5,               // Use with TailableCursor. If we are at the end of the data, block for a while rather
                                            // than returning no data. After a timeout period, we do return as normal.
    Exhaust         = 1 << 6,               // Stream the data down full blast in multiple “more” packages, on the assumption that the
                                            // client will fully read all data queried. Faster when you are pulling a lot of data and know
                                            // you want to pull it all down. Note: the client is not allowed to not read all the data
                                            // unless it closes the connection.
    Partial         = 1 << 7                // Get partial results from a mongos if some shards are down (instead of throwing an error)
};

using FieldSelector = std::map<std::string, int>;

enum class TailableCursor {Close,       LeaveOpen};
enum class Slave          {Fail,        OK};
enum class Oplog          {Replay,      NoReplay};
enum class Curser         {IdleTimeout, NoTimeout};
enum class Data           {Wait,        Timeout};
enum class Drain          {SingleBlock, All};
enum class Partial        {All,         Available};

struct QueryOptions
{
    QueryOptions()
        : tailableCursor(TailableCursor::Close)
        , slave(Slave::Fail)
        , oplog(Oplog::Replay)
        , curser(Curser::IdleTimeout)
        , data(Data::Wait)
        , drain(Drain::SingleBlock)
        , partial(Partial::All)
    {}

    QueryOptions& leaveOpen()        {tailableCursor = TailableCursor::LeaveOpen;return *this;}
    QueryOptions& slaveOK()          {slave          = Slave::OK;                return *this;}
    QueryOptions& opLogNoReplay()    {oplog          = Oplog::NoReplay;          return *this;}
    QueryOptions& curserNoTimeout()  {curser         = Curser::NoTimeout;        return *this;}
    QueryOptions& dataTimeout()      {data           = Data::Timeout;            return *this;}
    QueryOptions& drainAll()         {drain          = Drain::All;               return *this;}
    QueryOptions& partialAvailable() {partial        = Partial::Available;       return *this;}

    TailableCursor      tailableCursor;
    Slave               slave;
    Oplog               oplog;
    Curser              curser;
    Data                data;
    Drain               drain;
    Partial             partial;
};

template<typename Document>
class Op_Query
{
    MsgHeader       header;
    OP_QueryFlag    flags;
    std::string     fullCollectionName;
    std::int32_t    numberToSkip;
    std::int32_t    numberToReturn;
    Document        query;
    FieldSelector   returnFieldsSelector;
    public:
        template<typename... Args>
        Op_Query(std::string const& fullCollectionName, QueryOptions options, std::int32_t count, std::int32_t skip, Args&&... args);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_Query> const& data);
        friend std::ostream& operator<<(std::ostream& stream, Op_Query const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;
        void          handleOptions(QueryOptions const& options);
    protected:
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
};

}

ThorsAnvil_MakeEnumFlag(ThorsAnvil::DB::Mongo::OP_QueryFlag, empty, TailableCursor, SlaveOk, OplogReplay, NoCursorTimeout, AwaitData, Exhaust, Partial);

#include "Op_Query.tpp"

#endif
