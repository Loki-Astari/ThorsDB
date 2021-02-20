#ifndef THORSANVIL_DB_MONGO_OP_UPDATE_H
#define THORSANVIL_DB_MONGO_OP_UPDATE_H

/* $    Usage: Op_Update
 * $        Document:       Serializeable object that is sent/retrieved to/from Mongo.
 * $            Select:     Document that will be matched against records.
 * $            Update:     Document specifying fields to update.
 * $        connection:     connection to mongo DB or a stream.
 *
 * >    connection << send_Op_Update("db.collection" [, OP_DeleteFlag::<flag>] [, <Document: Select> [, <Document: Update>]]);
 */
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
        Op_Update(std::string fullCollectionName, OP_UpdateFlag flags, Selector&& selector = {}, Update&& update = {});

        void setCompression(std::int8_t compressionType)     {header.setCompression(compressionType);}

        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
    private:
        std::size_t   getSize() const;
};

template<typename Selector, typename Update>
std::ostream& operator<<(std::ostream& stream, Op_Update<Selector, Update> const& data);

template<typename Selector, typename Update>
Op_Update<Selector, Update> send_Op_Update(std::string fullCollectionName, Selector&& selector = {}, Update&& update = {});

template<typename Selector, typename Update>
Op_Update<Selector, Update> send_Op_Update(std::string fullCollectionName, OP_UpdateFlag flags, Selector&& selector = {}, Update&& update = {});

}

#include "Op_Update.tpp"

#endif
