#ifndef THORSANVIL_DB_MONGO_OP_INSERT_H
#define THORSANVIL_DB_MONGO_OP_INSERT_H

/* $    Usage: Op_Insert
 * $        Document:       Serializeable object that is sent/retrieved to/from Mongo.
 * $        connection:     connection to mongo DB or a stream.
 *
 * >    connection << send_Op_Insert("db.collection" [, OP_InsertFlag::<flag>] , <Document>);
 */

#include "Op.h"
#include "Util.h"
#include "View.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"

#include <string>
#include <iostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-insert
namespace ThorsAnvil::DB::Mongo
{

enum class OP_InsertFlag : std::int32_t
{
    empty           = 0,
    ContinueOnError = 1 << 0,   // If set, the database will not stop processing a bulk insert
                                // if one fails (eg due to duplicate IDs). This makes bulk insert
                                // behave similarly to a series of single inserts, except lastError
                                // will be set if any insert fails, not just the last one.
                                // If multiple errors occur, only the most recent will be
                                // reported by getLastError.
};
ThorsAnvil_MakeEnumFlag(OP_InsertFlag, empty, ContinueOnError);

template<typename View>
struct Op_Insert
{
    Op_MsgHeader            header;             // standard message header
    OP_InsertFlag           flags;
    std::string             fullCollectionName;
    View                    documents;

    public:
        Op_Insert(std::string fullCollectionName, OP_InsertFlag flags, View&& range);

        std::int32_t getMessageLength() const                    {return header.getMessageLength();}
        void         setCompression(std::int8_t compressionType) {header.setCompression(compressionType);}

        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
    private:
        std::size_t   getSize()                     const;
};

template<typename View>
std::ostream& operator<<(std::ostream& stream, Op_Insert<View> const& data);

template<typename Range>
Op_Insert<ViewType<Range>> send_Op_Insert(std::string fullCollectionName, Range&& r);

template<typename Range>
Op_Insert<ViewType<Range>> send_Op_Insert(std::string fullCollectionName, OP_InsertFlag flags, Range&& r);

}

#include "Op_Insert.tpp"

#endif
