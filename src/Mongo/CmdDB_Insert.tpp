#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#error  "This should only be included from CmdDB_Insert.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename I>
Insert<Document>::Insert(InsertOptions options, std::string collection, I begin, I end)
    : InsertOptional(std::move(options))
    , insert(std::move(collection))
    , documents(begin, end)
{}

}

#endif
