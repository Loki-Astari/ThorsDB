#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_DELETE_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_H
#error  "This should only be included from CmdDB_Delete.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
DeleteQuery<Document>::DeleteQuery(Document q)
    : q(std::move(q))
    , limit(0)
    , filter{{"collation", false}, {"hint", false}}
{}

template<typename Document>
Delete<Document>::Delete(DeleteOptions const& options, std::string collection, DeleteQuery<Document> doc)
    : DeleteOptional(options)
    , deleteFrom(std::move(collection))
    , deletes(1, std::move(doc))
{}

template<typename Document>
template<typename I>
Delete<Document>::Delete(DeleteOptions const& options, std::string collection, I begin, I end)
    : DeleteOptional(options)
    , deleteFrom(std::move(collection))
    , deletes(begin, end)
{}

}

#endif
