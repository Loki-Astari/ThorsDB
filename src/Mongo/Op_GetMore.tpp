#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_TPP
#define THORSANVIL_DB_MONGO_OP_GETMORE_TPP

#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_H
#error  "This should only be included from Op_GetMore.h"
#endif

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Opt, ValidGetOptions<Opt>>
Op_GetMore::Op_GetMore(std::string const& fullCollectionName, Opt&& options)
    : Op_GetMoreOptions(std::forward<Opt>(options))
    , header(OpCode::OP_GET_MORE)
    , fullCollectionName(fullCollectionName)
{
    header.prepareToSend(getSize());
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

}

#endif
