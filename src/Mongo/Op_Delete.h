#ifndef THORSANVIL_DB_MONGO_OP_DELETE_H
#define THORSANVIL_DB_MONGO_OP_DELETE_H

#include "Op.h"
#include "MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-delete
namespace ThorsAnvil::DB::Mongo
{

enum class OP_DeleteFlag : std::int32_t
{
    empty           = 0,
    SingleRemove    = 1,            // If set, the database will remove only the first matching
                                    // document in the collection. Otherwise all matching
                                    // documents will be removed.

                                    // 1-31 are reserved. Must be set to 0.
};

struct DeleteOptions
{
    OP_DeleteFlag       flags = OP_DeleteFlag::empty;
};

template<typename Document>
struct Op_Delete: public DeleteOptions
{
    MsgHeader               header;             // standard message header
    // std::int32_t            zero;               // 0 reserved for future use
    std::string             fullCollectionName; // "dbname.collectionname"
    Document                selector;           // query object.
    public:
        template<typename... Args>
        Op_Delete(std::string const& fullCollectionName, DeleteOptions const& flag, Args&&... args);
        template<typename... Args>
        Op_Delete(std::string const& fullCollectionName, DeleteOptions&& flag, Args&&... args);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_Delete> const& data);
        friend std::ostream& operator<<(std::ostream& stream, Op_Delete const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;
    protected:
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
};

}

ThorsAnvil_MakeEnumFlag(ThorsAnvil::DB::Mongo::OP_DeleteFlag, empty, SingleRemove);

#include "Op_Delete.tpp"

#endif
