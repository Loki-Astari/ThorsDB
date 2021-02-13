#ifndef THORSANVIL_DB_MONGO_OP_DELETE_H
#define THORSANVIL_DB_MONGO_OP_DELETE_H

#include "Op.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"

#include <string>
#include <iostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-delete

namespace ThorsAnvil::DB::Mongo
{

enum class OP_DeleteFlag : std::int32_t
{
    empty           = 0,
    SingleRemove    = 1 << 0,       // If set, the database will remove only the first matching
                                    // document in the collection. Otherwise all matching
                                    // documents will be removed.

                                    // 1-31 are reserved. Must be set to 0.
};
ThorsAnvil_MakeEnumFlag(OP_DeleteFlag, empty, SingleRemove);


template<typename Document>
class Op_Delete
{
    Op_MsgHeader            header;             // standard message header
    std::string             fullCollectionName;
    OP_DeleteFlag           flags;
    Document                selector;           // query object.

    public:
        Op_Delete(std::string fullCollectionName, OP_DeleteFlag flags, Document&& selector);

        std::ostream& print(std::ostream& stream)   const;
        std::ostream& printHR(std::ostream& stream) const;
    private:
        std::size_t   getSize()                     const;
};
template<typename Document>
std::ostream& operator<<(std::ostream& stream, Op_Delete<Document> const& data)
{
    return data.print(stream);
}

template<typename Document>
Op_Delete<Document> make_Op_Delete(std::string fullCollectionName, Document&& selector)
{
    return Op_Delete<Document>(std::move(fullCollectionName), OP_DeleteFlag::empty, std::forward<Document>(selector));
}

template<typename Document>
Op_Delete<Document> make_Op_Delete(std::string fullCollectionName, OP_DeleteFlag flags, Document&& selector)
{
    return Op_Delete<Document>(std::move(fullCollectionName), flags, std::forward<Document>(selector));
}

}

#include "Op_Delete.tpp"

#endif
