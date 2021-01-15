#ifndef THORSANVIL_DB_MONGO_OP_QUERY_TPP
#define THORSANVIL_DB_MONGO_OP_QUERY_TPP

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename... Args>
Op_Query<Document>::Op_Query(std::string const& fullCollectionName, QueryOptions options, std::int32_t count, std::int32_t skip, Args&&... args)
    : header(OpCode::OP_QUERY)
    , flags(OP_QueryFlag::empty)
    , fullCollectionName(fullCollectionName)
    , numberToSkip(skip)
    , numberToReturn(count)
    , query{std::move(args)...}
    , returnFieldsSelector{}
{
    handleOptions(options);
    header.prepareToSend(getSize());
}

template<typename Document>
void Op_Query<Document>::handleOptions(QueryOptions const& options)
{
    if (options.tailableCursor == TailableCursor::LeaveOpen)
    {
        flags |= OP_QueryFlag::TailableCursor;
    }
    if (options.slave == Slave::OK)
    {
        flags |= OP_QueryFlag::SlaveOk;;
    }
    if (options.oplog == Oplog::NoReplay)
    {
        flags |= OP_QueryFlag::OplogReplay;
    }
    if (options.curser == Curser::NoTimeout)
    {
        flags |= OP_QueryFlag::NoCursorTimeout;
    }
    if (options.data == Data::Timeout)
    {
        flags |= OP_QueryFlag::AwaitData;
    }
    if (options.drain == Drain::All)
    {
        flags |= OP_QueryFlag::Exhaust;
    }
    if (options.partial == Partial::Available)
    {
        flags |= OP_QueryFlag::Partial;
    }
}

template<typename Document>
std::size_t Op_Query<Document>::getSize() const
{
    std::size_t objectSize = sizeof(flags)
                           + fullCollectionName.size() + 1
                           + sizeof(numberToSkip)
                           + sizeof(numberToReturn)
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
           << make_LE(numberToSkip)
           << make_LE(numberToReturn)
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
           << "numberToSkip:        " << numberToSkip << "\n"
           << "numberToReturn:      " << numberToReturn << "\n"
           << "query:               " << ThorsAnvil::Serialize::jsonExporter(query) << "\n";
    if (!returnFieldsSelector.empty())
    {
        stream << "returnFieldsSelector:" << ThorsAnvil::Serialize::jsonExporter(returnFieldsSelector) << "\n";
    }
    return stream;
}

}

#endif
