#ifndef THORSANVIL_DB_MONGO_OP_DELETE_TPP
#define THORSANVIL_DB_MONGO_OP_DELETE_TPP

#ifndef THORSANVIL_DB_MONGO_OP_DELETE_H
#error  "This should only be included from Op_Delete.h"
#endif

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

#include <numeric>

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename... Args>
Op_Delete<Document>::Op_Delete(std::string const& fullCollectionName, OP_DeleteFlag flags, Args&&... args)
    : header(OpCode::OP_DELETE)
    , fullCollectionName(fullCollectionName)
    , flags(flags)
    , selector{std::forward<Args>(args)...}
{
    header.prepareToSend(getSize());
}
template<typename Document>
template<typename... Args>
Op_Delete<Document>::Op_Delete(std::string const& fullCollectionName, Args&&... args)
    : header(OpCode::OP_DELETE)
    , fullCollectionName(fullCollectionName)
    , flags(OP_DeleteFlag::empty)
    , selector{std::forward<Args>(args)...}
{
    header.prepareToSend(getSize());
}


template<typename Document>
std::size_t Op_Delete<Document>::getSize() const
{
    std::size_t objectSize = sizeof(std::int32_t)           // zero
                           + fullCollectionName.size() + 1
                           + sizeof(flags)
                           + ThorsAnvil::Serialize::bsonGetPrintSize(selector);
    return objectSize;
}

template<typename Document>
std::ostream& Op_Delete<Document>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(std::int32_t{0})
           << fullCollectionName << '\0'
           << make_LE(flags)
           << ThorsAnvil::Serialize::bsonExporter(selector);
    return stream;
}

template<typename Document>
std::ostream& Op_Delete<Document>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "zero: " << 0 << "\n"
           << "fullCollectionName: " << fullCollectionName << "\n"
           << flags << "\n"
           << ThorsAnvil::Serialize::jsonExporter(selector) << "\n";
    return stream;
}

}

#endif
