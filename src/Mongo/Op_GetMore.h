#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_H
#define THORSANVIL_DB_MONGO_OP_GETMORE_H

#include "Op.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>
#include <map>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-get-more
namespace ThorsAnvil::DB::Mongo
{

struct Op_GetMoreOptions
{
    std::int32_t    ret;
    std::int64_t    cursorID;
};

class Op_GetMore: public Op_GetMoreOptions
{
    Op_MsgHeader     header;
    std::string     fullCollectionName;
    public:
        Op_GetMore(std::string const& fullCollectionName, Op_GetMoreOptions const& options);
        Op_GetMore(std::string const& fullCollectionName, Op_GetMoreOptions&& options);

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
