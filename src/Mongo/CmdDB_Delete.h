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
    Document        q;
    std::size_t     limit;
    // Collation       collation;
    // std::string     hint;
};

template<typename Document>
struct Delete
{
    Delete(std::string const& collection, DeleteQuery<Document> const& doc, bool ordered, WriteConcern concern, std::string const& comment)
        : deleteFrom(collection)
        , deletes(1, doc)
        , ordered(ordered)
        //, writeConcern(concern)
        , comment(comment)
    {
        std::cerr << "Delete: One Query" << deletes.size() << "\n";
    }
    template<typename I>
    Delete(std::string const& collection, I begin, I end, bool ordered, WriteConcern concern, std::string const& comment)
        : deleteFrom(collection)
        , deletes(begin, end)
        , ordered(ordered)
        //, writeConcern(concern)
        , comment(comment)
    {
        std::cerr << "Delete: Many Query" << deletes.size() << "\n";
    }
    std::string                 deleteFrom;
    std::vector<DeleteQuery<Document>>    deletes;
    bool                        ordered;
    //WriteConcern                writeConcern;
    std::string                 comment;
};

template<typename Document>
using CmdDB_Delete      = CmdDB_Query<Delete<Document>>;

}

ThorsAnvil_Template_MakeTraitNameOverride(1, ThorsAnvil::DB::Mongo::Delete, {"deleteFrom", "delete"});
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Collation,                  locale, caseLevel, strength, numericOrdering, alternate, maxVariable, backwards);
ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::DeleteQuery,    q, limit); // , collation, hint);
ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::Delete,         deleteFrom, deletes, ordered, comment);


#endif
