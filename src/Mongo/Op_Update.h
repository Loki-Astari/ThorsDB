#ifndef THORSANVIL_DB_MONGO_OP_UPDATE_H
#define THORSANVIL_DB_MONGO_OP_UPDATE_H

#include "Op.h"
#include "MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>
#include <map>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-update
namespace ThorsAnvil::DB::Mongo
{

enum class OP_UpdateFlag : std::int32_t
{
    empty           = 0,
    Upsert          = 1 << 0,           // If set, the database will insert the supplied object into the collection if no matching document is found.
    MultiUpdate     = 1 << 1,           // If set, the database will update all matching objects in the collection. Otherwise only updates first matching document.
};

template<typename Selector, typename Update>
class Op_Update
{
    MsgHeader       header;
    // std::int32_t    zero;               // 0 reserved for future use
    std::string     fullCollectionName;
    OP_UpdateFlag   flags;
    Selector        selector;
    Update          update;
    public:
        Op_Update(std::string const& fullCollectionName, Selector&& selector, Update&& update);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_Update> const& data);
        friend std::ostream& operator<<(std::ostream& stream, Op_Update const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;
    protected:
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
};

}

ThorsAnvil_MakeEnumFlag(ThorsAnvil::DB::Mongo::OP_UpdateFlag, empty);

#include "Op_Update.tpp"

#endif
