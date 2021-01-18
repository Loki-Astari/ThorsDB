#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_H
#define THORSANVIL_DB_MONGO_CMD_DB_DELETE_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{


struct Collation
{
    std::string     locale;
    bool            caseLevel;
    int             strength;
    bool            numericOrdering;
    std::string     alternate;
    std::string     maxVariable;
    bool            backwards;
};

template<typename Document>
struct DeleteQuery
{
    DeleteQuery(Document const& q)
        : q(q)
        , limit(0)
    {}
    Document        q;
    std::size_t     limit;
    // Collation       collation;
    // std::string     hint;
};

template<typename Document>
struct Delete: public CmdDB_Base
{
    Delete(std::string const& collection, DeleteQuery<Document> const& doc)
        : deleteFrom(collection)
        , deletes(1, doc)
    {
        std::cerr << "Delete: One Query" << deletes.size() << "\n";
    }
    template<typename I>
    Delete(std::string const& collection, I begin, I end)
        : deleteFrom(collection)
        , deletes(begin, end)
    {
        std::cerr << "Delete: Many Query" << deletes.size() << "\n";
    }
    std::string                             deleteFrom;
    std::vector<DeleteQuery<Document>>      deletes;
};

template<typename Document>
using CmdDB_Delete      = CmdDB_Query<Delete<Document>>;

template<typename Document>
CmdDB_Delete<Document>
make_CmdDB_Delete(std::string const& db, std::string const& collection, QueryOptions&& options, Document const& doc)//, bool ordered, WriteConcern concern, bool byPass, std::string const& comment = "")
{
    return CmdDB_Delete<Document>(db, collection, std::move(options), DeleteQuery{doc});//, ordered, concern, byPass, comment);
}

}

ThorsAnvil_Template_MakeTraitNameOverride(1, ThorsAnvil::DB::Mongo::Delete, {"deleteFrom", "delete"});
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Collation,                  locale, caseLevel, strength, numericOrdering, alternate, maxVariable, backwards);
ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::DeleteQuery,    q, limit); // , collation, hint);
ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::Delete,         deleteFrom, deletes);


#endif
