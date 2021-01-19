#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_H
#define THORSANVIL_DB_MONGO_CMD_DB_DELETE_H

// https://docs.mongodb.com/manual/reference/command/delete/#dbcmd.delete

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{


template<typename Document>
struct DeleteQuery
{
    DeleteQuery(Document const& q)
        : q(q)
        , limit(0)
        , filter{{"collation", false}, {"hint", false}}
    {}
    Document        q;
    std::size_t     limit;
    Collation       collation;
    std::string     hint;

    std::map<std::string, bool>     filter;
};

template<typename Document>
struct Delete: public CmdDB_Base
{
    Delete(std::string const& collection, DeleteQuery<Document> const& doc)
        : deleteFrom(collection)
        , deletes(1, doc)
    {}
    template<typename I>
    Delete(std::string const& collection, I begin, I end)
        : deleteFrom(collection)
        , deletes(begin, end)
    {}
    std::string                             deleteFrom;
    std::vector<DeleteQuery<Document>>      deletes;
};

template<typename Document>
using CmdDB_Delete      = CmdDB_Query<Delete<Document>>;

template<typename Document>
CmdDB_Delete<Document>
make_CmdDB_Delete(std::string const& db, std::string const& collection, QueryOptions&& options, Document const& doc)
{
    return CmdDB_Delete<Document>(db, collection, std::move(options), DeleteQuery{doc});
}

}

ThorsAnvil_Template_MakeOverride(1, ThorsAnvil::DB::Mongo::Delete,      {"deleteFrom", "delete"});
ThorsAnvil_Template_MakeFilter(1, ThorsAnvil::DB::Mongo::DeleteQuery,   filter);

ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::DeleteQuery,    q, limit , collation, hint);
ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::Delete,         deleteFrom, deletes);


#endif
