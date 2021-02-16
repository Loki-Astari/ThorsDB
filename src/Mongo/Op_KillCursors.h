#ifndef THORSANVIL_DB_MONGO_OP_KILLCURSORS_H
#define THORSANVIL_DB_MONGO_OP_KILLCURSORS_H

/* $    Usage: Op_KillCursors
 * $        Document:       Serializeable object that is sent/retrieved to/from Mongo.
 * $        connection:     connection to mongo DB or a stream.
 * #        Note:           reply: will retrieve the cursor from the reply object.
 * #                           if none is provided will use the last reply on the connection.
 * #                           if none are provided and a boolean yes is used then all open
 * #                           cursors on that connection will be killed.
 *
 * >    connection << send_Op_KillCursors(reply...);
 * >    connection << send_Op_KillCursors([true]);
 */

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
Op_KillCursors send_Op_KillCursors(Op_Reply<Docs> const&... replys);

inline
Op_KillCursors send_Op_KillCursors(bool all = false);

}

#include "Op_KillCursors.tpp"

#endif
