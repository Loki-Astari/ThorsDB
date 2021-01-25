#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_DELETE_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_H
#error  "This should only be included from CmdDB_Delete.h"
#endif


namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
DeleteQuery<Document>::DeleteQuery(Document const& q)
    : q(q)
    , limit(0)
    , filter{{"collation", false}, {"hint", false}}
{}

template<typename Document>
template<typename Opt, ValidCmdDelOption<Opt>>
Delete<Document>::Delete(Opt&& options, std::string const& collection, DeleteQuery<Document> const& doc)
    : DeleteOptional(std::forward<Opt>(options))
    , deleteFrom(collection)
    , deletes(1, doc)
{}

template<typename Document>
template<typename Opt, ValidCmdDelOption<Opt>, typename I>
Delete<Document>::Delete(Opt&& options, std::string const& collection, I begin, I end)
    : DeleteOptional(std::forward<Opt>(options))
    , deleteFrom(collection)
    , deletes(begin, end)
{}

}

#endif
