#ifndef THORS_ANVIL_DB_MONGO_OP_QUERY_TPP
#define THORS_ANVIL_DB_MONGO_OP_QUERY_TPP

#ifndef THORS_ANVIL_DB_MONGO_OP_QUERY_H
#error  "This should only be included from Op_Query.h"
#endif

#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename Doc, NoOptions<Doc>, typename... Args>
Op_Query<Document>::Op_Query(std::string fullCollectionName, Op_QueryOptions const& options, Args&&... args)
    : Op_QueryOptions(options)
    , header(OpCode::OP_QUERY)
    , fullCollectionName(std::move(fullCollectionName))
    , query{std::forward<Args>(args)...}
{
    header.prepareToSend(getSize());
}

template<typename Document>
template<typename Doc, HasOptions<Doc>, typename... Args>
Op_Query<Document>::Op_Query(std::string fullCollectionName, Op_QueryOptions const& options, typename Doc::Options const& docOpt, Args&&... args)
    : Op_QueryOptions(options)
    , header(OpCode::OP_QUERY)
    , fullCollectionName(std::move(fullCollectionName))
    , query{docOpt, std::forward<Args>(args)...}
{
    header.prepareToSend(getSize());
}

template<typename Document>
Document& Op_Query<Document>::getQuery()
{
    return query;
}

template<typename Document>
std::size_t Op_Query<Document>::getSize() const
{
    std::size_t objectSize = sizeof(flags)
                           + fullCollectionName.size() + 1
                           + sizeof(skip)
                           + sizeof(ret)
                           + ThorsAnvil::Serialize::bsonGetPrintSize(query);
    if (!returnFieldsSelector.empty())
    {
        objectSize += ThorsAnvil::Serialize::bsonGetPrintSize(returnFieldsSelector);
    }
    return objectSize;
}

template<typename Document>
std::ostream& Op_Query<Document>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(flags)
           << fullCollectionName << '\0'
           << make_LE(skip)
           << make_LE(ret)
           << ThorsAnvil::Serialize::bsonExporter(query);
    if (!returnFieldsSelector.empty())
    {
        stream << ThorsAnvil::Serialize::bsonExporter(returnFieldsSelector);
    }
    return stream;
}

template<typename Document>
std::ostream& Op_Query<Document>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "flags:               " << flags << "\n"
           << "fullCollectionName:  " << fullCollectionName << "\n"
           << "numberToSkip:        " << skip << "\n"
           << "numberToReturn:      " << ret << "\n"
           << "query:               " << ThorsAnvil::Serialize::jsonExporter(query) << "\n";
    if (!returnFieldsSelector.empty())
    {
        stream << "returnFieldsSelector:" << ThorsAnvil::Serialize::jsonExporter(returnFieldsSelector) << "\n";
    }
    return stream;
}

template<typename Document>
std::ostream& operator<<(std::ostream& stream, Op_Query<Document> const& data)
{
    return data.print(stream);
}

}

#endif
