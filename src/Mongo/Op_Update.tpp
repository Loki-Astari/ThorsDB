#ifndef THORSANVIL_DB_MONGO_OP_UPDATE_TPP
#define THORSANVIL_DB_MONGO_OP_UPDATE_TPP

#ifndef THORSANVIL_DB_MONGO_OP_UPDATE_H
#error  "This should only be included from Op_Update.h"
#endif

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/BsonThor.h"
#include "ThorSerialize/JsonThor.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Selector, typename Update>
template<typename Opt, ValidUpdOptions<Opt>,
         typename Sel, ValidOption<Sel, Selector>,
         typename Upd, ValidOption<Upd, Update>
        >
Op_Update<Selector, Update>::Op_Update(std::string const& fullCollectionName, Opt&& options, Sel&& selector, Upd&& update)
    : Op_UpdateOptions(std::forward<Opt>(options))
    , header(OpCode::OP_UPDATE)
    , fullCollectionName(fullCollectionName)
    , selector(std::forward<Sel>(selector))
    , update(std::forward<Upd>(update))
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

}

#endif
