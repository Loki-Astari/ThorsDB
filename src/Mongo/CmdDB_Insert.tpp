#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#error  "This should only be included from CmdDB_Insert.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename Opt, ValidCmdInsOption<Opt>>
Insert<Document>::Insert(Opt&& options, std::string const& collection, Document const& doc)
    : InsertOptional(std::forward<Opt>(options))
    , insert(collection)
    , documents(1, doc)
{}

template<typename Document>
template<typename Opt, ValidCmdInsOption<Opt>, typename I>
Insert<Document>::Insert(Opt&& options, std::string const& collection, I begin, I end)
    : InsertOptional(std::forward<Opt>(options))
    , insert(collection)
    , documents(begin, end)
{}

}

#endif
