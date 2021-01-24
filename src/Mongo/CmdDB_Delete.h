#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_H
#define THORSANVIL_DB_MONGO_CMD_DB_DELETE_H

// https://docs.mongodb.com/manual/reference/command/delete/#dbcmd.delete

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{


template<typename Document>
struct DeleteQuery
{
    DeleteQuery(Document const& q);

    Document        q;
    std::size_t     limit;
    Collation       collation;
    std::string     hint;

    std::map<std::string, bool>     filter;
};

template<typename Document>
struct Delete
{
    public:
        template<typename I>
        Delete(std::string const& collection, I begin, I end);
        Delete(std::string const& collection, DeleteQuery<Document> const& doc);

        void unordered();
        void setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
    private:
        friend class ThorsAnvil::Serialize::Traits<Delete>;
        friend class ThorsAnvil::Serialize::Filter<Delete>;
        friend class ThorsAnvil::Serialize::Override<Delete>;
        std::map<std::string, bool> filter = {{"ordered", false}, {"writeConcern", false}};
        std::string                             deleteFrom;
        std::vector<DeleteQuery<Document>>      deletes;
        bool                                    ordered                  = true;
        WriteConcern                            writeConcern;
};

template<typename Document>
using CmdDB_Delete      = CmdDB_Query<Delete<Document>>;

template<typename Document>
CmdDB_Delete<Document>
make_CmdDB_Delete(std::string const& db, std::string const& collection, OP_QueryFlag flags, Document const& doc)
{
    return CmdDB_Delete<Document>(db, collection, flags, DeleteQuery{doc});
}

}

ThorsAnvil_Template_MakeFilter(1, ThorsAnvil::DB::Mongo::DeleteQuery,   filter);
ThorsAnvil_Template_MakeTrait(1,  ThorsAnvil::DB::Mongo::DeleteQuery,   q, limit , collation, hint);

ThorsAnvil_Template_MakeFilter(1,   ThorsAnvil::DB::Mongo::Delete,      filter);
ThorsAnvil_Template_MakeOverride(1, ThorsAnvil::DB::Mongo::Delete,      {"deleteFrom", "delete"});
ThorsAnvil_Template_MakeTrait(1,    ThorsAnvil::DB::Mongo::Delete,     deleteFrom, deletes, ordered, writeConcern);

#include "CmdDB_Delete.tpp"

#endif
