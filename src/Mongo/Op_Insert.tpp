#ifndef THORSANVIL_DB_MONGO_OP_QUERY_TPP
#define THORSANVIL_DB_MONGO_OP_QUERY_TPP

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

#include <numeric>

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename... Args>
Op_Insert<Document>::Op_Insert(std::string const& fullCollectionName, InsertError insertError, Args&&... args)
    : header(OpCode::OP_INSERT)
    , flags(OP_InsertFlag::empty)
    , fullCollectionName(fullCollectionName)
    , documents({Document{std::move(args)}...})
{
    if (insertError == InsertError::Cont)
    {
        flags |= OP_InsertFlag::ContinueOnError;
    }
    header.prepareToSend(getSize());
}

template<typename Document>
template<typename... Args>
Op_Insert<Document>::Op_Insert(std::string const& fullCollectionName, Args&&... args)
    : header(OpCode::OP_INSERT)
    , flags(OP_InsertFlag::ContinueOnError)
    , fullCollectionName(fullCollectionName)
    , documents({Document{std::move(args)}...})
{
    header.prepareToSend(getSize());
}


template<typename Document>
std::size_t Op_Insert<Document>::getSize() const
{
    std::size_t docSize = std::accumulate(std::begin(documents), std::end(documents),
                                          std::size_t{0},
                                          [](std::size_t sum, Document const& d){return sum + ThorsAnvil::Serialize::bsonGetPrintSize(d);}
                                         );
    std::size_t objectSize = sizeof(flags)
                           + fullCollectionName.size() + 1
                           + docSize;
    return objectSize;
}

template<typename Document>
std::ostream& Op_Insert<Document>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(flags)
           << fullCollectionName << '\0';
    for (auto const& d: documents)
    {
           stream << ThorsAnvil::Serialize::bsonExporter(d);
    }
    return stream;
}

template<typename Document>
std::ostream& Op_Insert<Document>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << flags << "\n"
           << "fullCollectionName: " << fullCollectionName << "\n"
           << "Size: " << documents.size() << "\n[";
    for (auto const& d: documents)
    {
        stream << ThorsAnvil::Serialize::jsonExporter(d);
    }
    stream << "\n]\n";
    return stream;
}

}

#endif