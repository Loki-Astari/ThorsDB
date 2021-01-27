#ifndef THORSANVIL_DB_MONGO_OP_REPLY_H
#define THORSANVIL_DB_MONGO_OP_REPLY_H

#include "Op.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"
#include <ostream>
#include <map>

// https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#database-response-messages
namespace ThorsAnvil::DB::Mongo
{

enum class OP_ReplyFlag : std::int32_t
{
    empty           = 0,
    CursorNotFound  = 1,                    // Is set when getMore is called but the cursor id is not valid at the server. Returned with zero results.
    QueryFailure    = 2,                    // Is set when query failed. Results consist of one document containing an “$err” field describing the failure.
    ShardConfigStale= 4,                    // Drivers should ignore this. Only mongos will ever see this set, in which case, it needs to update config from the server.
    AwaitCapable    = 8,                    // Is set when the server supports the AwaitData Query option.
                                            // If it doesn’t, a client should sleep a little between getMore’s of a Tailable cursor.
};

struct ErrorInfo
{
#pragma vera-pushoff
    double              ok;
    int                 code;
    std::string         $err;
    std::string         codeName;
#pragma vera-pop
};

template<typename Document>
struct Op_Reply
{
    using Documents = std::vector<Document>;

    Op_MsgHeader            header;                 // standard message header
    OP_ReplyFlag            responseFlags;          // bit vector - see details above
    std::int64_t            cursorID;               // cursor id if client needs to do get more's
    std::int32_t            startingFrom;           // where in the cursor this reply is starting
    std::int32_t            numberReturned;         // number of documents in the reply
    std::vector<Document>   documents;              // documents
    ErrorInfo               errorInfo;
    public:
        Op_Reply()
            : header(OpCode::OP_REPLY)
            , responseFlags(OP_ReplyFlag::empty)
            , cursorID(0)
            , startingFrom(0)
            , numberReturned(0)
            , errorInfo{1.0, 0, "", ""}
        {}
        virtual ~Op_Reply() {}

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_Reply> const& reply);
        friend std::ostream& operator<<(std::ostream& stream, Op_Reply const& reply) {return reply.print(stream);}
        friend std::istream& operator>>(std::istream& stream, Op_Reply& reply)       {return reply.parse(stream);}

        std::size_t     getDocumentCount()              const {return documents.size();}
        Document const& getDocument(std::size_t size)   const {return documents[size];}

        explicit operator   bool()              const;
        virtual bool        isOk()              const;
        virtual std::string getHRErrorMessage() const;
    protected:
        std::istream& parse(std::istream& stream);
        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
};

}

ThorsAnvil_MakeEnumFlag(ThorsAnvil::DB::Mongo::OP_ReplyFlag, empty, CursorNotFound, QueryFailure, ShardConfigStale, AwaitCapable);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ErrorInfo,  ok, code, codeName, $err);

#include "Op_Reply.tpp"

#endif
