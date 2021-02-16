#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_H
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_H

// https://docs.mongodb.com/manual/reference/command/findAndModify/#findandmodify

/* $    Usage: CmdDB_FindModify
 * $        Document:       Serializeable object that is sent/retrieved to/from Mongo.
 * $            Update:         Describes how to update the found document.
 * $            Find:           Description of the search criteria.
 * $            Sort:           Description of the sort criteria:               Default normal sort
 * $        connection:     connection to mongo DB or a stream.
 * $        Op_ptions:          See: Op_Query.h
 * $        FindModifyOptions:  See: below
 *
 * >    connection << send_CmdDB_FindDelete("db", "collection" [, Op_Query_Options] [, FindModifyOptions], <Document:Find> [, <Document:Sort>]);
 * >    connection >> get_CmdDB_FindDelete(std::unique_ptr<Document>);
 * >    // OR
 * >    connection << send_CmdDB_FindUpdate("db", "collection" [, Op_Query_Options] [, FindModifyOptions], <Document:Update>, <Document:Find> [, <Document:Sort>]);
 * >    connection >> get_CmdDB_FindUpdate(std::unique_ptr<Document>);
 */

#include "CmdDB.h"
#include "CmdDB_FindCommon.h"
#include "CmdDB_Query.h"
#include "CmdDB_Reply.h"
#include "ThorSerialize/MongoUtility.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

struct FindModifyOptions
{
    bool                        returnModified              = false;
    std::map<std::string, int>  fields;
    bool                        upsert                      = false;
    bool                        bypassDocumentValidation    = false;
    WriteConcern                writeConcern;
    Collation                   collation;
    //std::vector<Filter>         arrayFilters;
    std::string                 hint;
    std::string                 comment;
};

class FindModifyOptional
{
    friend class ThorsAnvil::Serialize::Traits<FindModifyOptional>;
    friend class ThorsAnvil::Serialize::Filter<FindModifyOptional>;
    std::map<std::string, bool>     filter  = {
                                                {"returnModified", false},
                                                {"fields", false}, {"upsert", "false"},
                                                {"bypassDocumentValidation", false},
                                                {"writeConcern", false}, {"collation", false},
                                                {"arrayFilters", false}, {"hint", false},
                                                {"comment", false}
                                              };

    bool                        returnModified;     // new
    std::map<std::string, int>  fields;
    bool                        upsert;
    bool                        bypassDocumentValidation;
    WriteConcern                writeConcern;
    Collation                   collation;
    //std::vector<Filter>         arrayFilters;     // Don't understand how to represent this.
    std::string                 hint;
    std::string                 comment;

    public:
        FindModifyOptional(FindModifyOptions const& options);

        void setNew(bool val = true);
        void addFileds(std::initializer_list<std::string> fieldNames);
        void setUpsert(bool val = true);
        void byPass(bool val = true);
        void setWriteConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
        // TODO setCollation
        // TODO setArrayFilter
        void addHint(std::string val);
        void setComment(std::string val);
};

template<typename Find, typename Sort, typename Update>
class FindModify: public FindModifyOptional
{
    friend class ThorsAnvil::Serialize::Traits<FindModify>;
    friend class ThorsAnvil::Serialize::Filter<FindModify>;
    std::map<std::string, bool>     filter  = {
                                                {"query", false}, {"sort", false},
                                                {"remove", false}, {"update", false},
                                              };

    std::string         findAndModify;
    Find                query;
    Sort                sort;
    bool                remove;
    Update              update;

    public:
        using Options = FindModifyOptions;

        FindModify(FindModifyOptions const& options, std::string collection, bool remove, Find&& find, Sort&& sort);
        FindModify(FindModifyOptions const& options, std::string collection, Update&& update, Find&& find, Sort&& sort);
};

struct LastErrorObject
{
    //using ThorsAnvil::Serialize::MongoUtility::ObjectID;

    ThorsAnvil::Serialize::MongoUtility::ObjectID    upserted;
    bool        updatedExisting             = false;
};

template<typename Document>
struct FindModifyReply: public CmdReplyBase
{
    using UserData = std::unique_ptr<Document>;
    using Reference = std::reference_wrapper<UserData>;

    FindModifyReply(UserData& v)
        : value(v)
    {}

    LastErrorObject             lastErrorObject;
    Reference                   value;
};

template<typename Document>
using CmdDB_FindModifyReply     = CmdDB_Reply<FindModifyReply<Document>>;

template<typename Find, typename Sort, typename Update>
using CmdDB_FindModify        = CmdDB_Query<FindModify<Find, Sort, Update>>;

template<typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, Find&& find, Sort&& sort = {});

template<typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, FindModifyOptions const& findModOpt, Find&& find, Sort&& sort = {});

template<typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, Op_QueryOptions const& options, Find&& find, Sort&& sort = {});

template<typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, NoUpdate> send_CmdDB_FindDelete(std::string db, std::string collection, Op_QueryOptions const& options, FindModifyOptions const& findModOpt, Find&& find, Sort&& sort = {});

template<typename Update, typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, Update&& update, Find&& find, Sort&& sort = {});

template<typename Update, typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, Op_QueryOptions const& options, Update&& update, Find&& find, Sort&& sort = {});

template<typename Update, typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, FindModifyOptions const& findModOpt, Update&& update, Find&& find, Sort&& sort = {});

template<typename Update, typename Find, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, Update> send_CmdDB_FindUpdate(std::string db, std::string collection, Op_QueryOptions const& options, FindModifyOptions const& findModOpt, Update&& update, Find&& find, Sort&& sort = {});

template<typename Document>
CmdDB_FindModifyReply<Document> get_CmdDB_FindDeleteReply(std::unique_ptr<Document>& doc)   {return CmdDB_FindModifyReply<Document>(doc);}

template<typename Document>
CmdDB_FindModifyReply<Document> get_CmdDB_FindUpdateReply(std::unique_ptr<Document>& doc)   {return CmdDB_FindModifyReply<Document>(doc);}

}

ThorsAnvil_MakeOverride(ThorsAnvil::DB::Mongo::FindModifyOptional,                  {"returnModified", "new"});
ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::FindModifyOptional,                    filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::FindModifyOptional,                     returnModified,
                                                                                    fields, upsert, bypassDocumentValidation,
                                                                                    writeConcern, collation,
                                                                                    hint, comment);
ThorsAnvil_Template_MakeFilter(3, ThorsAnvil::DB::Mongo::FindModify,                filter);
ThorsAnvil_Template_ExpandTrait(3, ThorsAnvil::DB::Mongo::FindModifyOptional,
                                   ThorsAnvil::DB::Mongo::FindModify,               findAndModify, remove, update, query, sort);

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::LastErrorObject,                        updatedExisting, upserted);
ThorsAnvil_Template_ExpandTrait(1, ThorsAnvil::DB::Mongo::CmdReplyBase,
                                   ThorsAnvil::DB::Mongo::FindModifyReply,          lastErrorObject, value);

#include "CmdDB_FindModify.tpp"

#endif
