#ifndef THORSANVIL_DB_MONGO_OP_INSERT_H
#define THORSANVIL_DB_MONGO_OP_INSERT_H

#include "Op.h"
#include "MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-insert
namespace ThorsAnvil::DB::Mongo
{

enum class OP_InsertFlag : std::int32_t
{
    empty           = 0,
    ContinueOnError = 1,        // If set, the database will not stop processing a bulk insert
                                // if one fails (eg due to duplicate IDs). This makes bulk insert
                                // behave similarly to a series of single inserts, except lastError
                                // will be set if any insert fails, not just the last one.
                                // If multiple errors occur, only the most recent will be
                                // reported by getLastError.
};

template<typename Document>
struct Op_Insert
{
    MsgHeader               header;             // standard message header
    OP_InsertFlag           flags;              // bit vector - see above
    std::string             fullCollectionName; // "dbname.collectionname"
    std::vector<Document>   documents;          // one or more documents to insert into the collection
    public:
        template<typename... Args>
        Op_Insert(std::string const& fullCollectionName, Args&&... args);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_Insert> const& data);
        friend std::ostream& operator<<(std::ostream& stream, Op_Insert const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;
    protected:
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
};

}

ThorsAnvil_MakeEnumFlag(ThorsAnvil::DB::Mongo::OP_InsertFlag, ContinueOnError);

#include "Op_Insert.tpp"

#endif
