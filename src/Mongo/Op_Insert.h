#ifndef THORSANVIL_DB_MONGO_OP_INSERT_H
#define THORSANVIL_DB_MONGO_OP_INSERT_H

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
        Op_Insert(std::string fullCollectionName, View&& range);
        Op_Insert(std::string fullCollectionName, OP_InsertFlag flags, View&& range);

        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
    private:
        std::size_t   getSize()                     const;
};

template<typename View>
std::ostream& operator<<(std::ostream& stream, Op_Insert<View> const& data)
{
    return data.print(stream);
}

template<typename Range>
Op_Insert<ViewType<Range>> make_Op_Insert(std::string fullCollectionName, Range&& r)
{
    return Op_Insert<ViewType<Range>>(std::move(fullCollectionName), OP_InsertFlag::empty, make_XView(std::forward<Range>(r)));
}
template<typename Range>
Op_Insert<ViewType<Range>> make_Op_Insert(std::string fullCollectionName, OP_InsertFlag flags, Range&& r)
{
    return Op_Insert<ViewType<Range>>(std::move(fullCollectionName), flags, make_XView(std::forward<Range>(r)));
}

}

#include "Op_Insert.tpp"

#endif
