#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_H
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_H

// https://docs.mongodb.com/manual/reference/command/find/#dbcmd.findAndModify

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
        FindModifyOptional(FindModifyOptions options);

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

        FindModify(FindModifyOptions options, std::string collection, bool remove, Find&& find, Sort&& sort)
            : FindModifyOptional(std::move(options))
            , findAndModify(std::move(collection))
            , query(std::forward<Find>(find))
            , sort(std::forward<Sort>(sort))
            , remove(remove)
        {
            filter["query"]     = !std::is_same_v<FindAll, Base<Find>>;
            filter["sort"]      = !std::is_same_v<DefaultSort, Base<Sort>>;
            filter["remove"]    = true;
        }
        FindModify(FindModifyOptions options, std::string collection, Update update, Find&& find, Sort&& sort)
            : FindModifyOptional(std::move(options))
            , findAndModify(std::move(collection))
            , query(std::forward<Find>(find))
            , sort(std::forward<Sort>(sort))
            , remove(false)
            , update(std::forward<Update>(update))
        {
            filter["query"]     = !std::is_same_v<FindAll, Base<Find>>;
            filter["sort"]      = !std::is_same_v<DefaultSort, Base<Sort>>;
            filter["update"]    = true;
        }
};

struct LastErrorObject
{
    //using ThorsAnvil::Serialize::MongoUtility::ObjectID;

    ThorsAnvil::Serialize::MongoUtility::ObjectID    upserted;
    bool        updatedExisting             = false;
};

template<typename Document>
struct FindModifyReply
{
    LastErrorObject             lastErrorObject;
    std::unique_ptr<Document>   value;
    double                      ok                      = 0.0;

    bool isOk() const                       {return ok;}
    std::string getHRErrorMessage() const   {return "XXX";}
};

template<typename Document>
using CmdDB_FindModifyReply     = CmdDB_Reply<FindModifyReply<Document>>;

template<typename Find, typename Sort, typename Update>
using CmdDB_FindModify        = CmdDB_Query<FindModify<Find, Sort, Update>>;

template<typename Find = FindAll, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, NoUpdate> make_CmdDB_FindDelete(std::string db, std::string collection, FindModifyOptions findModOpt = {}, Find&& find = {}, Sort&& sort = {})
{
    return CmdDB_FindModify<Find, Sort, NoUpdate>(std::move(db), std::move(collection), {}, std::move(findModOpt), true, std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Find = FindAll, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, NoUpdate> make_CmdDB_FindDelete(std::string db, std::string collection, Op_QueryOptions options, FindModifyOptions findModOpt = {}, Find&& find = {}, Sort&& sort = {})
{
    return CmdDB_FindModify<Find, Sort, NoUpdate>(std::move(db), std::move(collection), std::move(options), std::move(findModOpt), true, std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Update, typename Find = FindAll, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, Update> make_CmdDB_FindUpdate(std::string db, std::string collection, FindModifyOptions findModOpt = {}, Update&& update = {}, Find&& find = {}, Sort&& sort = {})
{
    return CmdDB_FindModify<Find, Sort, Update>(std::move(db), std::move(collection), {}, std::move(findModOpt), std::forward<Update>(update), std::forward<Find>(find), std::forward<Sort>(sort));
}

template<typename Update, typename Find = FindAll, typename Sort = DefaultSort>
CmdDB_FindModify<Find, Sort, Update> make_CmdDB_FindUpdate(std::string db, std::string collection, Op_QueryOptions options, FindModifyOptions findModOpt = {}, Update&& update = {}, Find&& find = {}, Sort&& sort = {})
{
    return CmdDB_FindModify<Find, Sort, Update>(std::move(db), std::move(collection), std::move(options), std::move(findModOpt), std::forward<Update>(update), std::forward<Find>(find), std::forward<Sort>(sort));
}

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
ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::FindModifyReply,            lastErrorObject, value, ok);

#include "CmdDB_FindModify.tpp"

#endif
