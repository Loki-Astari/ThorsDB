#ifndef THORS_ANVIL_DB_MONGO_OP_QUERY_H
#define THORS_ANVIL_DB_MONGO_OP_QUERY_H

/*
 * $    Usage:  Op_Query
 *
 * >        This is not designed to be used directly. See HandShake.h for example
 * >        I would expect you to define the Kind and what goes into it as part of a typedef.
 */

#include "Op.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"

#include <map>
#include <string>
#include <iostream>

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
ThorsAnvil_MakeEnumFlag(OP_QueryFlag, empty, TailableCursor, SlaveOk, OplogReplay, NoCursorTimeout, AwaitData, Exhaust, Partial);

using FieldSelector = std::map<std::string, int>;

struct Op_QueryOptions
{
    OP_QueryFlag    flags                   = OP_QueryFlag::empty;
    std::int32_t    skip                    = 0;
    std::int32_t    ret                     = 1;
    FieldSelector   returnFieldsSelector;
};

template<typename Document>
class Op_Query: public Op_QueryOptions
{
    Op_MsgHeader            header;
    std::string             fullCollectionName;
    Document                query;

    public:
        template<typename Doc = Document, NoOptions<Doc> = true, typename... Args>
        Op_Query(std::string fullCollectionName, Op_QueryOptions const& options, Args&&... args);
        template<typename Doc = Document, HasOptions<Doc> = true, typename... Args>
        Op_Query(std::string fullCollectionName, Op_QueryOptions const& options, typename Doc::Options const& docOpt, Args&&... args);

        Document& getQuery();

        std::int32_t getMessageLength() const                    {return header.getMessageLength();}
        void         setCompression(std::int8_t compressionType) {header.setCompression(compressionType);}

        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
    private:
        std::size_t   getSize() const;
};

template<typename Document>
std::ostream& operator<<(std::ostream& stream, Op_Query<Document> const& data);

}

#include "Op_Query.tpp"

#endif
