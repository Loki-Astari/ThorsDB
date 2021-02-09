#ifndef THORSANVIL_DB_MONGO_OP_REPLY_H
#define THORSANVIL_DB_MONGO_OP_REPLY_H

#include "Op.h"
#include "Util.h"
#include "View.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"

#include <string>
#include <iostream>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#database-response-messages

namespace ThorsAnvil::DB::Mongo
{

enum class OP_ReplyFlag : std::int32_t
{
    empty           = 0,
    CursorNotFound  = 1 << 0,               // Is set when getMore is called but the cursor id is not valid at the server. Returned with zero results.
    QueryFailure    = 1 << 1,               // Is set when query failed. Results consist of one document containing an “$err” field describing the failure.
    ShardConfigStale= 1 << 2,               // Drivers should ignore this. Only mongos will ever see this set, in which case, it needs to update config from the server.
    AwaitCapable    = 1 << 3,               // Is set when the server supports the AwaitData Query option.
                                            // If it doesn’t, a client should sleep a little between getMore’s of a Tailable cursor.
};
ThorsAnvil_MakeEnumFlag(OP_ReplyFlag, empty, CursorNotFound, QueryFailure, ShardConfigStale, AwaitCapable);

struct ErrorInfo
{
#pragma vera-pushoff
    double              ok          = 1.0;
    int                 code        = 0;
    std::string         $err        = "";
    std::string         codeName    = "";
#pragma vera-pop
    std::string         getHRErrorMessage() const;
};

struct MongoException: public std::runtime_error
{
    ErrorInfo   info;
    MongoException(std::string const& message, ErrorInfo const& info)
        : runtime_error(message)
        , info(info)
    {}
};

class Op_GetMore;
class Op_KillCursor;
class MongoConnection;

template<typename Document, typename View = ViewType<Document>>
struct Op_Reply
{
    Op_MsgHeader            header;                 // standard message header
    OP_ReplyFlag            responseFlags;          // bit vector - see details above
    CursorId                cursorID;               // cursor id if client needs to do get more's
    std::int32_t            startingFrom;           // where in the cursor this reply is starting
    std::int32_t            numberReturned;         // number of documents in the reply
    View                    documents;              // documents
    ErrorInfo               errorInfo;
    // Allow the Op_GetMore/Op_KillCursor/MongoConnection  class to extract the cursorID from the reply
    friend class Op_GetMore;
    friend class Op_KillCursor;
    friend class MongoConnection;

    public:
        Op_Reply(Document& data);
        virtual ~Op_Reply() {}

        explicit operator   bool()              const;
        virtual bool        isOk()              const;
        virtual std::string getHRErrorMessage() const;

        std::istream& parse(std::istream& stream);
        std::istream& parseAndThrow(std::istream& stream);
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;

        [[deprecated("This function is for unit testing only. Do not use it. I will make it do silly things in the future to mess with people that use this function")]]
        void cursorIDSetForTest(CursorId value)                {cursorID = value;}  // Used for testing
};
template<typename Document, typename View = ViewType<Document>>
std::ostream& operator<<(std::ostream& stream, Op_Reply<Document, View> const& reply)
{
    return reply.print(stream);
}

template<typename Document, typename View = ViewType<Document>>
std::istream& operator>>(std::istream& stream, Op_Reply<Document, View>&  reply)
{
    return reply.parse(stream);
}

template<typename Document, typename View = ViewType<Document>>
std::istream& operator>>(std::istream& stream, Op_Reply<Document, View>&& reply)
{
    return reply.parseAndThrow(stream);
}

template<typename Object, ValidSingle<Object> = true>
Op_Reply<Object> make_Op_Reply(Object& object)
{
    return Op_Reply<Object>(object);
}
template<typename Range, ValidContainer<Range> = true>
Op_Reply<Range> make_Op_Reply(Range& container)
{
    return Op_Reply<Range>(container);// | FilterBackInserter{});
}

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ErrorInfo,  ok, code, codeName, $err);

#include "Op_Reply.tpp"

#endif
