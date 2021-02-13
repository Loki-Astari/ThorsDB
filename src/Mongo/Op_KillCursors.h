#ifndef THORSANVIL_DB_MONGO_OP_KILLCURSORS_H
#define THORSANVIL_DB_MONGO_OP_KILLCURSORS_H

#include "Op.h"
#include "Op_MsgHeader.h"
#include "Op_Reply.h"

#include <string>
#include <vector>
#include <iostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-kill-cursors

namespace ThorsAnvil::DB::Mongo
{

template<typename T>
class CursorExtractor;

class Op_KillCursors
{
    Op_MsgHeader            header;
    std::int32_t            numberOfCursorIDs;
    std::vector<CursorId>   cursorIDs;
    friend class CursorExtractor<Op_KillCursors>;

    public:
        template<typename... Docs>
        Op_KillCursors(Op_Reply<Docs> const&... replys);
        Op_KillCursors(bool all = false);

        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
        friend std::ostream& operator<<(std::ostream& stream, Op_KillCursors const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;
};

template<typename... Docs>
Op_KillCursors make_Op_KillCursors(Op_Reply<Docs> const&... replys)   {return Op_KillCursors(replys...);}

inline
Op_KillCursors make_Op_KillCursors(bool all = false)                  {return Op_KillCursors(all);}

}

#include "Op_KillCursors.tpp"

#endif
