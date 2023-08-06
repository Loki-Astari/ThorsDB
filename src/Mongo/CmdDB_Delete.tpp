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

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, Document doc)
{
    return CmdDB_Delete<Document>(std::move(db), std::move(collection), {}, {}, DeleteQuery{std::move(doc)});
}

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, DeleteOptions const& deleteOpt, Document doc)
{
    return CmdDB_Delete<Document>(std::move(db), std::move(collection), {}, deleteOpt, DeleteQuery{std::move(doc)});
}

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, Op_QueryOptions const& options, Document doc)
{
    return CmdDB_Delete<Document>(std::move(db), std::move(collection), options, {}, DeleteQuery{std::move(doc)});
}

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, Op_QueryOptions const& options, DeleteOptions const& deleteOpt, Document doc)
{
    return CmdDB_Delete<Document>(std::move(db), std::move(collection), options, deleteOpt, DeleteQuery{std::move(doc)});
}
}

#endif
