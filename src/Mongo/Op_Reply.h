#ifndef THORSANVIL_DB_MONGO_OP_REPLY_H
#define THORSANVIL_DB_MONGO_OP_REPLY_H

/* $    Usage: Op_Reply
 * $        Document:       Serializeable object that is sent/retrieved to/from Mongo.
 * $        View:           A container that holds Documents
 * $        connection:     connection to mongo DB or a stream.
 *
 * >    connection << send_Op_QueryDerivedType(...);
 * >    connection >> get_Op_Reply(<Document or View>);
 */

#include "Op.h"
#include "Util.h"
#include "View.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"

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
    friend std::ostream& operator<<(std::ostream& stream, MongoException const& data)
    {
        return stream << data.what() << "\n" << ThorsAnvil::Serialize::jsonExporter(data.info);
    }
};

class Op_GetMore;
class Op_KillCursor;
class MongoConnection;

template<typename View>
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
        Op_Reply(View&& view);
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

template<typename View>
std::ostream& operator<<(std::ostream& stream, Op_Reply<View> const& reply);

template<typename View>
std::istream& operator>>(std::istream& stream, Op_Reply<View>& reply);

template<typename View>
std::istream& operator>>(std::istream& stream, Op_Reply<View>&& reply);

template<typename Range>
Op_Reply<ViewType<Range>> get_Op_Reply(Range&& range);

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ErrorInfo,  ok, code, codeName, $err);

#include "Op_Reply.tpp"

#endif
