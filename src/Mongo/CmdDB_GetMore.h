#ifndef THORSANVIL_DB_MONGO_CMD_DB_GET_MORE_H
#define THORSANVIL_DB_MONGO_CMD_DB_GET_MORE_H

// https://docs.mongodb.com/manual/reference/command/getMore/#getmore

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
        GetMoreOptional(GetMoreOptions options);

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

        GetMore(GetMoreOptions options, std::string collection, CursorId cursor);
    private:
        friend class ThorsAnvil::Serialize::Traits<GetMore>;
        friend class CursorExtractor<CmdDB_GetMore>;

        // Required Parameters
        CursorId            getMore;
        std::string         collection;
};

template<typename Document>
using CmdDB_GetMoreReply    = CmdDB_FindReplyBase<CursorNext<Document>>;

// Helper functions to return object
inline
CmdDB_GetMore make_CmdDB_GetMore(std::string db, std::string collection, GetMoreOptions getMoreOpt)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), {}, std::move(getMoreOpt), -1);
}
inline
CmdDB_GetMore make_CmdDB_GetMore(std::string db, std::string collection, Op_QueryOptions options = {}, GetMoreOptions getMoreOpt = {})
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), std::move(options), std::move(getMoreOpt), -1);
}
template<typename Cursor>
inline
CmdDB_GetMore make_CmdDB_GetMore(std::string db, std::string collection, CmdDB_FindReplyBase<Cursor> const& reply)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), Op_QueryOptions{}, {}, reply.findData.cursor.id);
}
template<typename Cursor>
inline
CmdDB_GetMore make_CmdDB_GetMore(std::string db, std::string collection, GetMoreOptions getMoreOpt, CmdDB_FindReplyBase<Cursor> const& reply)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), Op_QueryOptions{}, std::move(getMoreOpt), reply.findData.cursor.id);
}
template<typename Cursor>
inline
CmdDB_GetMore make_CmdDB_GetMore(std::string db, std::string collection, Op_QueryOptions options, GetMoreOptions getMoreOpt, CmdDB_FindReplyBase<Cursor> const& reply)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), std::move(options), std::move(getMoreOpt), reply.findData.cursor.id);
}

}

// Serialization Traits for objects that go over the wire.
ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::GetMoreOptional,           filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::GetMoreOptional,            batchSize, maxTimeMS, comment);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::GetMoreOptional,
                       ThorsAnvil::DB::Mongo::GetMore,                  getMore, collection);

#endif