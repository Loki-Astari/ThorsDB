#ifndef THORSANVIL_DB_MONGO_CMD_DB_GET_MORE_H
#define THORSANVIL_DB_MONGO_CMD_DB_GET_MORE_H

// https://docs.mongodb.com/manual/reference/command/getMore/#getmore

/* $    Usage: CmdDB_GetMore
 * $        Document:           Serializeable object that is sent/retrieved to/from Mongo.
 * $        connection:         connection to mongo DB or a stream.
 * $        Op_QueryOptions:    See: Op_Query.h
 * $        GetMoreOptions:     See: below
 * $        Reply:              object returned from CmdDB_Find or CmdDB_GetMore
 * $                            If provided the cursor is extracted from this, otherwise the cursor
 * $                            is retrived from the connection that keeps track of the last cursor used.
 *
 * >    connection << send_CmdDB_GetMore("db", "collection" [, Op_Query_Options] [, GetMoreOptions], [, CmdDB_FindReply]);
 * >    connection >> get_CmdDB_GetMoreReply(std::vector<Document>);    // CmdDB_Find
 */

#include "CmdDB.h"
#include "CmdDB_Query.h"
#include "CmdDB_Reply.h"
#include "CmdDB_Find.h"

#include <map>
#include <string>

namespace ThorsAnvil::DB::Mongo
{

struct GetMoreOptions
{
    // Options
    std::int32_t        batchSize       = 0;
    std::int32_t        maxTimeMS       = 0;
    std::string         comment;
};

struct GetMoreOptional
{
    public:
        GetMoreOptional(GetMoreOptions const& options);

        // Options setting functions.
        void setBatchSize(std::size_t val);
        void setMaxTimeout(std::size_t val);
        void setComment(std::string c);
    private:
        friend class ThorsAnvil::Serialize::Traits<GetMoreOptional>;
        friend class ThorsAnvil::Serialize::Filter<GetMoreOptional>;
        std::map<std::string, bool>     filter;

        // Options
        std::int32_t        batchSize       = 0;
        std::int32_t        maxTimeMS       = 0;
        std::string         comment;
};

template<typename T>
class CursorExtractor;

class GetMore;

using CmdDB_GetMore         = CmdDB_Query<GetMore>;

class GetMore: public GetMoreOptional
{
    public:
        using Options = GetMoreOptions;

        GetMore(GetMoreOptions const& options, std::string collection, CursorId cursor);
    private:
        friend class ThorsAnvil::Serialize::Traits<GetMore>;
        friend class CursorExtractor<CmdDB_GetMore>;

        // Required Parameters
        CursorId            getMore;
        std::string         collection;
};

template<typename Document>
using CmdDB_GetMoreReply    = CmdDB_FindReplyBase<CursorNext<Base<Document>>>;

// Helper functions to return object
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, GetMoreOptions const& getMoreOpt = {});

CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, Op_QueryOptions const& options, GetMoreOptions const& getMoreOpt = {});

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, CmdDB_FindReplyBase<Cursor> const& reply);

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, GetMoreOptions const& getMoreOpt, CmdDB_FindReplyBase<Cursor> const& reply);

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, Op_QueryOptions const& options, CmdDB_FindReplyBase<Cursor> const& reply);

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, Op_QueryOptions const& options, GetMoreOptions const& getMoreOpt, CmdDB_FindReplyBase<Cursor> const& reply);

template<typename Document>
CmdDB_GetMoreReply<Document> get_CmdDB_GetMoreReply(std::vector<Document>& data)  {return CmdDB_GetMoreReply<Document>(data);}

}

// Serialization Traits for objects that go over the wire.
ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::GetMoreOptional,           filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::GetMoreOptional,            batchSize, maxTimeMS, comment);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::GetMoreOptional,
                       ThorsAnvil::DB::Mongo::GetMore,                  getMore, collection);

#include "CmdDB_GetMore.tpp"

#endif
