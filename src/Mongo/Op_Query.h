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
    TailableCursor  = 2,                    // TailableCursor means cursor is not closed when the last data is retrieved.
                                            // Rather, the cursor marks the final object’s position.
                                            // You can resume using the cursor later, from where it was located, if more data were received.
                                            // Like any “latent cursor”, the cursor may become invalid at some point (CursorNotFound)
    SlaveOk         = 4,                    // Allow query of replica slave. Normally these return an error except for namespace “local”.
    OplogReplay     = 8,                    // Starting in MongoDB 4.4, you need not specify this flag because the optimization automatically
                                            // happens for eligible queries on the oplog. See oplogReplay for more information.
    NoCursorTimeout = 16,                   // The server normally times out idle cursors after an inactivity period (10 minutes)
                                            // to prevent excess memory use. Set this option to prevent that.
    AwaitData       = 32,                   // Use with TailableCursor. If we are at the end of the data, block for a while rather
                                            // than returning no data. After a timeout period, we do return as normal.
    Exhaust         = 64,                   // Stream the data down full blast in multiple “more” packages, on the assumption that the
                                            // client will fully read all data queried. Faster when you are pulling a lot of data and know
                                            // you want to pull it all down. Note: the client is not allowed to not read all the data
                                            // unless it closes the connection.
    Partial         = 128                   // Get partial results from a mongos if some shards are down (instead of throwing an error)
};

using FieldSelector = std::map<std::string, int>;

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
        Op_Query(std::string const& fullCollectionName, Args&&... args);

        friend std::ostream& operator<<(std::ostream& stream, Op_Query& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_Query> const& data);
    private:
        std::size_t   getSize()                     const;
        std::ostream& print(std::ostream& stream);
        std::ostream& printHR(std::ostream& stream);
};

}

ThorsAnvil_MakeEnumFlag(ThorsAnvil::DB::Mongo::OP_QueryFlag, empty, TailableCursor, SlaveOk, OplogReplay, NoCursorTimeout, AwaitData, Exhaust, Partial);

#if defined(HEADER_ONLY) && HEADER_ONLY == 1
#include "Op_Query.source"
#endif

#ifndef COVERAGE_TEST
#include "Op_Query.tpp"
#endif

#endif
