#ifndef THORSANVIL_DB_MONGO_OP_GETMORE_H
#define THORSANVIL_DB_MONGO_OP_GETMORE_H

/* $    Usage: Op_GetMore
 * $        Document:       Serializeable object that is sent/retrieved to/from Mongo.
 * $        connection:     connection to mongo DB or a stream.
 * #        Note:           reply: will retrieve the cursor from the reply object.
 * #                           if none is provided will use the last reply on the connection.
 * #                           if you pass to a stream without a reply object it may be funky.
 *
 * >    connection << send_Op_GetMore("db.collection" [, reply] [, <number to return]);
 * >    connection >> get_Op_Reply(<Document>)
 */
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

        std::int32_t getMessageLength() const                    {return header.getMessageLength();}
        void         setCompression(std::int8_t compressionType) {header.setCompression(compressionType);}

        std::ostream& print(std::ostream& stream)   const;
        std::ostream& printHR(std::ostream& stream) const;
        friend std::ostream& operator<<(std::ostream& stream, Op_GetMore const& data) {return data.print(stream);}
    private:
        std::size_t   getSize()                     const;

};

template<typename Document>
Op_GetMore send_Op_GetMore(std::string fullCollectionName, Op_Reply<Document> const& reply, std::int32_t ret = 101);

inline
Op_GetMore send_Op_GetMore(std::string fullCollectionName, std::int32_t ret = 101);

}

#include "Op_GetMore.tpp"

#endif
