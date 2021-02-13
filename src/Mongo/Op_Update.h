#ifndef THORSANVIL_DB_MONGO_OP_UPDATE_H
#define THORSANVIL_DB_MONGO_OP_UPDATE_H

#include "Op.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"

#include <string>
#include <iostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-update

namespace ThorsAnvil::DB::Mongo
{

enum class OP_UpdateFlag : std::int32_t
{
    empty           = 0,
    Upsert          = 1 << 0,           // If set, the database will insert the supplied object into the collection if no matching document is found.
    MultiUpdate     = 1 << 1,           // If set, the database will update all matching objects in the collection. Otherwise only updates first matching document.
};
ThorsAnvil_MakeEnumFlag(OP_UpdateFlag, empty);

template<typename Selector, typename Update>
class Op_Update
{
    Op_MsgHeader            header;
    std::string             fullCollectionName;
    OP_UpdateFlag           flags;
    Selector                selector;
    Update                  update;

    public:
        Op_Update(std::string fullCollectionName, Selector&& selector = {}, Update&& update = {});
        Op_Update(std::string fullCollectionName, OP_UpdateFlag flags, Selector&& selector = {}, Update&& update = {});

        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
    private:
        std::size_t   getSize() const;
};

template<typename Selector, typename Update>
std::ostream& operator<<(std::ostream& stream, Op_Update<Selector, Update> const& data)
{
    return data.print(stream);
}

template<typename Selector, typename Update>
Op_Update<Selector, Update> make_Op_Update(std::string fullCollectionName, Selector&& selector = {}, Update&& update = {})
{
    return Op_Update<Selector, Update>(std::move(fullCollectionName), std::forward<Selector>(selector), std::forward<Update>(update));
}
template<typename Selector, typename Update>
Op_Update<Selector, Update> make_Op_Update(std::string fullCollectionName, OP_UpdateFlag flags, Selector&& selector = {}, Update&& update = {})
{
    return Op_Update<Selector, Update>(std::move(fullCollectionName), flags, std::forward<Selector>(selector), std::forward<Update>(update));
}

}

#include "Op_Update.tpp"

#endif
