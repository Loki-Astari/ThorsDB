#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_H
#define THORSANVIL_DB_MONGO_OP_GETMORE_H

#include "Op.h"
#include "Op_MsgHeader.h"
#include "Op_Reply.h"

#include <string>
#include <iostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-get-more

namespace ThorsAnvil::DB::Mongo
{

template<typename T>
class CursorExtractor;

class Op_GetMore
{
    Op_MsgHeader            header;
    std::string             fullCollectionName;
    std::int32_t            ret;
    CursorId                cursorID;
    friend class CursorExtractor<Op_GetMore>;

    public:
        template<typename Document>
        Op_GetMore(std::string fullCollectionName, Op_Reply<Document> const& reply, std::int32_t ret = 101);
        Op_GetMore(std::string fillCollectionName, std::int32_t ret = 101);

        std::ostream& print(std::ostream& stream)   const;
        std::ostream& printHR(std::ostream& stream) const;
        friend std::ostream& operator<<(std::ostream& stream, Op_GetMore const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;

};

template<typename Document>
inline
Op_GetMore make_Op_GetMore(std::string fullCollectionName, Op_Reply<Document> const& reply, std::int32_t ret = 101)
{
    return Op_GetMore(std::move(fullCollectionName), reply, ret);
}
inline
Op_GetMore make_Op_GetMore(std::string fullCollectionName, std::int32_t ret = 101)
{
    return Op_GetMore(std::move(fullCollectionName), ret);
}

}

#include "Op_GetMore.tpp"

#endif
