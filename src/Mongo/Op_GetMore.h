#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_H
#define THORSANVIL_DB_MONGO_OP_GETMORE_H

#include "Op.h"
#include "MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>
#include <map>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-get-more
namespace ThorsAnvil::DB::Mongo
{

class Op_GetMore
{
    MsgHeader       header;
    // std::int32_t    zero;               // 0 reserved for future use
    std::string     fullCollectionName;
    std::int32_t    numberToReturn;
    std::int64_t    cursorID;
    public:
        Op_GetMore(std::string const& fullCollectionName, std::int32_t count, std::int64_t cursor);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_GetMore> const& data);
        friend std::ostream& operator<<(std::ostream& stream, Op_GetMore const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;
    protected:
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
};

}

#include "Op_GetMore.tpp"

#endif
