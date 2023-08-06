#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_TPP
#define THORSANVIL_DB_MONGO_OP_GETMORE_TPP

#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_H
#error  "This should only be included from Op_GetMore.h"
#endif

#if 0
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
Op_GetMore::Op_GetMore(std::string fullCollectionName, Op_Reply<Document> const& reply, std::int32_t ret)
    : header(OpCode::OP_GET_MORE)
    , fullCollectionName(std::move(fullCollectionName))
    , ret(ret)
    , cursorID(reply.cursorID)
{
    header.prepareToSend(getSize());
}

inline
Op_GetMore::Op_GetMore(std::string fullCollectionName, std::int32_t ret)
    : header(OpCode::OP_GET_MORE)
    , fullCollectionName(std::move(fullCollectionName))
    , ret(ret)
    , cursorID(-1)
{
    // Update CursorExtractor<Op_GetMore>
    // As this will change the size of the object.
}

inline
std::size_t Op_GetMore::getSize() const
{
    std::size_t objectSize = sizeof(std::int32_t)               // ZERO
                           + fullCollectionName.size() + 1
                           + sizeof(ret)
                           + sizeof(cursorID);
    return objectSize;
}

inline
std::ostream& Op_GetMore::print(std::ostream& stream) const
{
    stream << header
           << make_LE(std::int32_t{0})
           << fullCollectionName << '\0'
           << make_LE(ret)
           << make_LE(cursorID);
    return stream;
}

inline
std::ostream& Op_GetMore::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "ZERO:                " << 0 << " (reserved)\n"
           << "fullCollectionName:  " << fullCollectionName << "\n"
           << "numberToReturn:      " << ret << "\n"
           << "cursorID:            " << cursorID << "\n";
    return stream;
}

template<typename Document>
Op_GetMore send_Op_GetMore(std::string fullCollectionName, Op_Reply<Document> const& reply, std::int32_t ret)
{
    return Op_GetMore(std::move(fullCollectionName), reply, ret);
}

inline
Op_GetMore send_Op_GetMore(std::string fullCollectionName, std::int32_t ret)
{
    return Op_GetMore(std::move(fullCollectionName), ret);
}

}

#endif
