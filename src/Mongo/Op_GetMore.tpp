#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_TPP
#define THORSANVIL_DB_MONGO_OP_GETMORE_TPP

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
Op_GetMore<Document>::Op_GetMore(std::string const& fullCollectionName, std::int32_t count, std::int64_t cursor)
    : header(OpCode::OP_GET_MORE)
    , fullCollectionName(fullCollectionName)
    , numberToReturn(count)
    , cursorID(cursor)
{
    header.prepareToSend(getSize());
}

template<typename Document>
std::size_t Op_GetMore<Document>::getSize() const
{
    std::size_t objectSize = sizeof(std::int32_t)               // ZERO
                           + fullCollectionName.size() + 1
                           + sizeof(numberToReturn)
                           + sizeof(cursorID);
    return objectSize;
}

template<typename Document>
std::ostream& Op_GetMore<Document>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(0)
           << fullCollectionName << '\0'
           << make_LE(numberToReturn)
           << make_LE(cursorID);
    return stream;
}

template<typename Document>
std::ostream& Op_GetMore<Document>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "ZERO:                " << 0 << " (reserved)\n"
           << "fullCollectionName:  " << fullCollectionName << "\n"
           << "numberToReturn:      " << numberToReturn << "\n"
           << "cursorID:            " << cursorID << "\n";
    return stream;
}

}

#endif
