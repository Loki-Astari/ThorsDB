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

struct DeleteOptions
{
        bool                                    ordered                  = true;
        WriteConcern                            writeConcern;
};
class DeleteOptional
{
    public:
        DeleteOptional(DeleteOptions const& options);
        DeleteOptional(DeleteOptions&& options);

        void unordered(bool val = true);
        void setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
    private:
        void updateFilter();

        friend class ThorsAnvil::Serialize::Traits<DeleteOptional>;
        friend class ThorsAnvil::Serialize::Filter<DeleteOptional>;
        std::map<std::string, bool> filter = {{"ordered", false}, {"writeConcern", false}};
        bool                                    ordered;
        WriteConcern                            writeConcern;
};

struct DeleteQueryOptions: public Op_QueryOptions, public DeleteOptions {};

template<typename Actual>
using ValidCmdDelOption = ValidOption<Actual, DeleteQueryOptions>;

template<typename Document>
struct Delete: public DeleteOptional
{
    public:
        template<typename Opt = DeleteQueryOptions, ValidCmdDelOption<Opt> = true, typename I>
        Delete(Opt&& options, std::string const& collection, I begin, I end);
        template<typename Opt = DeleteQueryOptions, ValidCmdDelOption<Opt> = true>
        Delete(Opt&& options, std::string const& collection, DeleteQuery<Document> const& doc);

    private:
        friend class ThorsAnvil::Serialize::Traits<Delete>;
        friend class ThorsAnvil::Serialize::Override<Delete>;
        std::string                             deleteFrom;
        std::vector<DeleteQuery<Document>>      deletes;
};

template<typename Document>
using CmdDB_Delete      = CmdDB_Query<Delete<Document>>;

template<typename Opt = DeleteQueryOptions, ValidCmdDelOption<Opt> = true, typename Document>
CmdDB_Delete<Document>
make_CmdDB_Delete(std::string const& db, std::string const& collection, Opt&& options, Document const& doc)
{
    return CmdDB_Delete<Document>(db, collection, std::forward<Opt>(options), DeleteQuery{doc});
}

}

ThorsAnvil_Template_MakeFilter(1, ThorsAnvil::DB::Mongo::DeleteQuery,       filter);
ThorsAnvil_Template_MakeTrait(1,  ThorsAnvil::DB::Mongo::DeleteQuery,       q, limit , collation, hint);

ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::DeleteOptional,                filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DeleteOptional,                 ordered, writeConcern);

ThorsAnvil_Template_MakeOverride(1, ThorsAnvil::DB::Mongo::Delete,          {"deleteFrom", "delete"});
ThorsAnvil_Template_ExpandTrait(1,  ThorsAnvil::DB::Mongo::DeleteOptional, ThorsAnvil::DB::Mongo::Delete,     deleteFrom, deletes);

#include "CmdDB_Delete.tpp"

#endif
