#ifndef THORS_ANVIL_DB_MONGO_OP_INSERT_TPP
#define THORS_ANVIL_DB_MONGO_OP_INSERT_TPP

#ifndef THORS_ANVIL_DB_MONGO_OP_INSERT_H
#error  "This should only be included from Op_Insert.h"
#endif

#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename View>
Op_Insert<View>::Op_Insert(std::string fullCollectionName, OP_InsertFlag flags, View&& view)
    : header(OpCode::OP_INSERT)
    , flags(flags)
    , fullCollectionName(std::move(fullCollectionName))
    , documents(std::forward<View>(view))
{
    header.prepareToSend(getSize());
}

template<typename View>
std::size_t Op_Insert<View>::getSize() const
{
    std::size_t docSize = std::accumulate(std::begin(documents), std::end(documents),
                                          std::size_t{0},
                                          [](std::size_t sum, auto const& d){return sum + ThorsAnvil::Serialize::bsonGetPrintSize(d);}
                                         );
    std::size_t objectSize = sizeof(flags)
                           + fullCollectionName.size() + 1
                           + docSize;
    return objectSize;
}

template<typename View>
std::ostream& Op_Insert<View>::print(std::ostream& stream) const
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

template<typename View>
std::ostream& Op_Insert<View>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << flags << "\n"
           << "fullCollectionName: " << fullCollectionName << "\n"
           << "Size: " << std::size(documents) << "\n[";
    for (auto const& d: documents)
    {
        stream << ThorsAnvil::Serialize::jsonExporter(d);
    }
    stream << "\n]\n";
    return stream;
}

template<typename View>
std::ostream& operator<<(std::ostream& stream, Op_Insert<View> const& data)
{
    return data.print(stream);
}

template<typename Range>
Op_Insert<ViewType<Range>> send_Op_Insert(std::string fullCollectionName, Range&& r)
{
    return Op_Insert<ViewType<Range>>(std::move(fullCollectionName), OP_InsertFlag::empty, make_XView(std::forward<Range>(r)));
}

template<typename Range>
Op_Insert<ViewType<Range>> send_Op_Insert(std::string fullCollectionName, OP_InsertFlag flags, Range&& r)
{
    return Op_Insert<ViewType<Range>>(std::move(fullCollectionName), flags, make_XView(std::forward<Range>(r)));
}

}

#endif
