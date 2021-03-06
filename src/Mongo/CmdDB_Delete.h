#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_H
#define THORSANVIL_DB_MONGO_CMD_DB_DELETE_H

// https://docs.mongodb.com/manual/reference/command/delete/#dbcmd.delete

/* $    Usage: CmdDB_Delete
 * $        Document:           Serializeable object that is sent/retrieved to/from Mongo.
 * $        connection:         connection to mongo DB or a stream.
 * $        Op_QueryOptions:    See: Op_Query.h
 * $        DeleteOptions:      See: below
 * $        count:              std::size_t number of documents deleted.
 *
 * >    connection << send_CmdDB_Delete("db", "collection" [, Op_Query_Options] [, DeleteOptions] [,<Document>]);
 * <    connection >> get_CmdDB_DeleteReply(count);
 */

#include "BaseCmd.h"
#include "CmdDB_Query.h"
#include "CmdDB_Reply.h"

#include <map>
#include <vector>
#include <string>

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
struct DeleteQuery
{
    DeleteQuery(Document q);

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

template<typename Document>
struct Delete: public DeleteOptional
{
    public:
        using Options = DeleteOptions;

        template<typename I>
        Delete(DeleteOptions const& options, std::string collection, I begin, I end);
        Delete(DeleteOptions const& options, std::string collection, DeleteQuery<Document> doc);

    private:
        friend class ThorsAnvil::Serialize::Traits<Delete>;
        friend class ThorsAnvil::Serialize::Override<Delete>;
        std::string                             deleteFrom;
        std::vector<DeleteQuery<Document>>      deletes;
};

template<typename Document>
using CmdDB_Delete      = CmdDB_Query<Delete<Document>>;
using CmdDB_DeleteReply = CmdDB_Reply<CmdReply>;

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, Document doc);

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, DeleteOptions const& deleteOpt, Document doc);

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, Op_QueryOptions const& options, Document doc);

template<typename Document>
CmdDB_Delete<Document> send_CmdDB_Delete(std::string db, std::string collection, Op_QueryOptions const& options, DeleteOptions const& deleteOpt, Document doc);

inline CmdDB_DeleteReply get_CmdDB_DeleteReply(std::size_t& count) {return CmdDB_DeleteReply(count);}

}

ThorsAnvil_Template_MakeFilter(1, ThorsAnvil::DB::Mongo::DeleteQuery,       filter);
ThorsAnvil_Template_MakeTrait(1,  ThorsAnvil::DB::Mongo::DeleteQuery,       q, limit , collation, hint);

ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::DeleteOptional,                filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DeleteOptional,                 ordered, writeConcern);

ThorsAnvil_Template_MakeOverride(1, ThorsAnvil::DB::Mongo::Delete,          {"deleteFrom", "delete"});
ThorsAnvil_Template_ExpandTrait(1,  ThorsAnvil::DB::Mongo::DeleteOptional, ThorsAnvil::DB::Mongo::Delete,     deleteFrom, deletes);

#include "CmdDB_Delete.tpp"

#endif
