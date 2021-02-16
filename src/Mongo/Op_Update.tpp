#ifndef THORSANVIL_DB_MONGO_OP_UPDATE_TPP
#define THORSANVIL_DB_MONGO_OP_UPDATE_TPP

#ifndef THORSANVIL_DB_MONGO_OP_UPDATE_H
#error  "This should only be included from Op_Update.h"
#endif

#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Selector, typename Update>
Op_Update<Selector, Update>::Op_Update(std::string fullCollectionName, OP_UpdateFlag flags, Selector&& selector, Update&& update)
    : header(OpCode::OP_UPDATE)
    , fullCollectionName(std::move(fullCollectionName))
    , flags(flags)
    , selector(std::forward<Selector>(selector))
    , update(std::forward<Update>(update))
{
    header.prepareToSend(getSize());
}

template<typename Selector, typename Update>
std::size_t Op_Update<Selector, Update>::getSize() const
{
    std::size_t objectSize = sizeof(std::int32_t)               // ZERO
                           + fullCollectionName.size() + 1
                           + sizeof(flags)
                           + ThorsAnvil::Serialize::bsonGetPrintSize(selector)
                           + ThorsAnvil::Serialize::bsonGetPrintSize(update);
    return objectSize;
}

template<typename Selector, typename Update>
std::ostream& Op_Update<Selector, Update>::print(std::ostream& stream) const
{
    stream << header
           << make_LE(std::int32_t{0})
           << fullCollectionName << '\0'
           << make_LE(flags)
           << ThorsAnvil::Serialize::bsonExporter(selector)
           << ThorsAnvil::Serialize::bsonExporter(update);
    return stream;
}

template<typename Selector, typename Update>
std::ostream& Op_Update<Selector, Update>::printHR(std::ostream& stream) const
{
    stream << make_hr(header)
           << "ZERO:                " << 0 << " (reserved)\n"
           << "fullCollectionName:  " << fullCollectionName << "\n"
           << "selector:            " << ThorsAnvil::Serialize::jsonExporter(selector) << "\n"
           << "update:              " << ThorsAnvil::Serialize::jsonExporter(update)   << "\n";
    return stream;
}

template<typename Selector, typename Update>
std::ostream& operator<<(std::ostream& stream, Op_Update<Selector, Update> const& data)
{
    return data.print(stream);
}

template<typename Selector, typename Update>
Op_Update<Selector, Update> send_Op_Update(std::string fullCollectionName, Selector&& selector, Update&& update)
{
    return Op_Update<Selector, Update>(std::move(fullCollectionName), OP_UpdateFlag::empty, std::forward<Selector>(selector), std::forward<Update>(update));
}

template<typename Selector, typename Update>
Op_Update<Selector, Update> send_Op_Update(std::string fullCollectionName, OP_UpdateFlag flags, Selector&& selector, Update&& update)
{
    return Op_Update<Selector, Update>(std::move(fullCollectionName), flags, std::forward<Selector>(selector), std::forward<Update>(update));
}

}

#endif
