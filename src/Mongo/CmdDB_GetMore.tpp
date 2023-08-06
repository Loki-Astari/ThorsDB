#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_GET_MORE_TPP
#define THORS_ANVIL_DB_MONGO_CMD_DB_GET_MORE_TPP

#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_GET_MORE_H
#error  "This should only be included from CmdDB_GetMore.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, CmdDB_FindReplyBase<Cursor> const& reply)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), {}, {}, reply.reply.cursor.id);
}

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, GetMoreOptions const& getMoreOpt, CmdDB_FindReplyBase<Cursor> const& reply)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), {}, getMoreOpt, reply.reply.cursor.id);
}

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, Op_QueryOptions const& options, CmdDB_FindReplyBase<Cursor> const& reply)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), options, {}, reply.reply.cursor.id);
}

template<typename Cursor>
CmdDB_GetMore send_CmdDB_GetMore(std::string db, std::string collection, Op_QueryOptions const& options, GetMoreOptions const& getMoreOpt, CmdDB_FindReplyBase<Cursor> const& reply)
{
    using namespace std::string_literals;
    return CmdDB_GetMore(std::move(db), std::move(collection), options, getMoreOpt, reply.reply.cursor.id);
}

}

#endif
