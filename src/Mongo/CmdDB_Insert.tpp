#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#error  "This should only be included from CmdDB_Insert.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename View>
Insert<View>::Insert(InsertOptions const& options, std::string collection, View&& view)
    : InsertOptional(options)
    , insert(std::move(collection))
    , documents(std::forward<View>(view))
{}

template<typename Range>
CmdDB_Insert<ViewType<Range>> send_CmdDB_Insert(std::string db, std::string collection, Range&& range)
{
    return CmdDB_Insert<ViewType<Range>>(std::move(db), std::move(collection), {}, {}, make_XView(std::forward<Range>(range)));
}

template<typename Range>
CmdDB_Insert<ViewType<Range>> send_CmdDB_Insert(std::string db, std::string collection, InsertOptions const& options, Range&& range)
{
    return CmdDB_Insert<ViewType<Range>>(std::move(db), std::move(collection), {}, options, make_XView(std::forward<Range>(range)));
}

template<typename Range>
CmdDB_Insert<ViewType<Range>> send_CmdDB_Insert(std::string db, std::string collection, Op_QueryOptions const& options, Range&& range)
{
    return CmdDB_Insert<ViewType<Range>>(std::move(db), std::move(collection), options, {}, make_XView(std::forward<Range>(range)));
}

template<typename Range>
CmdDB_Insert<ViewType<Range>>
send_CmdDB_Insert(std::string db, std::string collection, Op_QueryOptions const& options, InsertOptions const& insertOpt, Range&& range)
{
    return CmdDB_Insert<ViewType<Range>>(std::move(db), std::move(collection), options, insertOpt, make_XView(std::forward<Range>(range)));
}

}

#endif
