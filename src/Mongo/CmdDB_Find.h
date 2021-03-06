#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_H
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_H

// https://docs.mongodb.com/manual/reference/command/find/#dbcmd.find

/* $    Usage: CmdDB_Find
 * $        Document:           Serializeable object that is sent/retrieved to/from Mongo.
 * $            Find:           Document that specifies what to find:   Default All
 * $            Sort:           Document that specifies fields to sort: Default Normal.
 * $        connection:         connection to mongo DB or a stream.
 * $        Op_QueryOptions:    See: Op_Query.h
 * $        DeleteOptions:      See: below
 *
 * >    connection << send_CmdDB_Find("db", "collection" [, Op_Query_Options] [, FindOptions] [,<Document: Find> [, <Document:Sort>]]);
 * >    // OR
 * >    connection << send_CmdDB_FindAllSort("db", "collection" [, Op_Query_Options] [, FindOptions] [, <Document:Sort>]);
 * >    // Followed by
 * >    connection >> get_CmdDB_FindReply(std::vector<Document>)
 * >
 * >    // Optionally
 * >    connection << send_CmdDB_GetMore("db", "collection" [, Op_Query_Options] [, GetMoreOptions], [, CmdDB_FindReply]);
 * >    connection >> get_CmdDB_GetMoreReply(std::vector<Document>);    // Appends
 */

#include "BaseCmd.h"
#include "CmdDB_FindCommon.h"
#include "CmdDB_Query.h"
#include "CmdDB_Reply.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

struct FindOptions
{
    std::map<std::string, int>  projection;
    std::string                 hint;
    std::size_t                 skip                = 0;
    std::size_t                 limit               = 0;
    std::size_t                 batchSize           = 101;
    bool                        singleBatch         = false;
    std::string                 comment;
    std::size_t                 maxTimeMS           = 0;
    ReadConcern                 readConcern;
    std::map<std::string, int>  max;
    std::map<std::string, int>  min;
    bool                        returnKey           = false;
    bool                        showRecordId        = false;
    bool                        tailable            = false;
    bool                        awaitData           = false;
    bool                        noCursorTimeout     = false;
    bool                        allowPartialResults = false;
    Collation                   collation;
    bool                        allowDiskUse        = false;
};

struct FindOptional
{
    public:
        FindOptional(FindOptions const& options);

        void addFileds(std::initializer_list<std::string> fieldNames);
        void addHint(std::string hint);
        void setSkip(std::size_t val);
        void setLimit(std::size_t val);
        void setBatchSize(std::size_t val);
        void oneBatch(bool val = true);
        void setComment(std::string val);
        void setMaxTimeout(std::size_t val);
        void addReadConcern(ReadConcernLevel val);
        void addMax(std::string field, int val);
        void addMin(std::string field, int val);
        void justKeys(bool val = false);
        void showId(bool val = true);
        void tailableCursor(bool val = true);
        void tailedCursorAwait(bool val = true);
        void setNoCursorTimeout(bool val = true);
        void setAllowPartialResults(bool val = true);
        // TODO setCollation
        void useDisk(bool val = true);
    private:
        void updateFilter();
        std::map<std::string, bool>     optionsFilter ={
                                                        {"projection", false},
                                                        {"hint", false},
                                                        {"skip", false}, {"limit", false}, {"batchSize", false}, {"singleBatch", false},
                                                        {"comment", false},
                                                        {"maxTimeMS", false},
                                                        {"readConcern", false},
                                                        {"max", false}, {"min", false},
                                                        {"returnKey", false}, {"showRecordId", false},
                                                        {"tailable", false}, {"awaitData", false}, {"noCursorTimeout", false}, {"allowPartialResults", false},
                                                        {"collation", false},
                                                        {"allowDiskUse", false}
                                                       };
        friend class ThorsAnvil::Serialize::Traits<FindOptional>;
        friend class ThorsAnvil::Serialize::Filter<FindOptional>;
        std::map<std::string, int>  projection;
        std::string                 hint;
        std::size_t                 skip;
        std::size_t                 limit;
        std::size_t                 batchSize;
        bool                        singleBatch;
        std::string                 comment;
        std::size_t                 maxTimeMS;
        ReadConcern                 readConcern;
        std::map<std::string, int>  max;
        std::map<std::string, int>  min;
        bool                        returnKey;
        bool                        showRecordId;
        bool                        tailable;
        bool                        awaitData;
        bool                        noCursorTimeout;
        bool                        allowPartialResults;
        Collation                   collation;
        bool                        allowDiskUse;
};

template<typename Filter, typename Sort>
class Find: public FindOptional
{
    public:
        using Options = FindOptions;

        Find(FindOptions const& options, std::string collection, Filter&& filter, Sort&& sort);
    private:
        void updateFilter();
        friend class ThorsAnvil::Serialize::Traits<Find>;
        friend class ThorsAnvil::Serialize::Filter<Find>;

        std::string                 find;
        Filter                      filter;
        Sort                        sort;
        std::map<std::string, bool>     findFilter = {{"filter", false}, {"sort", false}};
};

template<typename Filter, typename Sort>
using CmdDB_Find            = CmdDB_Query<Find<Filter, Sort>>;

template<typename Document>
using CmdDB_FindReply       = CmdDB_FindReplyBase<CursorFirst<Base<Document>>>;

template<typename Filter = FindAll, typename Sort = DefaultSort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, Filter&& filter = Filter{}, Sort&& sort = Sort{});

template<typename Filter = FindAll, typename Sort = DefaultSort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, FindOptions const& findOpt, Filter&& filter = Filter{}, Sort&& sort = Sort{});

template<typename Filter = FindAll, typename Sort = DefaultSort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, Op_QueryOptions const& options, Filter&& filter = Filter{}, Sort&& sort = Sort{});

template<typename Filter = FindAll, typename Sort = DefaultSort>
CmdDB_Find<Filter, DefaultSort> send_CmdDB_Find(std::string db, std::string collection, Op_QueryOptions const& options, FindOptions const& findOpt, Filter&& filter = Filter{}, Sort&& sort = Sort{});

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, Sort&& sort);

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, FindOptions const& findOpt, Sort&& sort);

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, Op_QueryOptions const& options, Sort&& sort);

template<typename Sort>
CmdDB_Find<FindAll, Sort> send_CmdDB_FindAllSort(std::string db, std::string collection, Op_QueryOptions const& options, FindOptions const&findOpt, Sort&& sort);

template<typename Document>
CmdDB_FindReply<Document> get_CmdDB_FindReply(std::vector<Document>& data)  {return CmdDB_FindReply<Document>(data);}

}

// Find Action
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ReadConcern,                level);
ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::FindOptional,              optionsFilter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::FindOptional,               projection, hint, skip, limit, batchSize,
                                                                        singleBatch, comment, maxTimeMS, readConcern, max, min, returnKey,
                                                                        showRecordId, tailable, awaitData, noCursorTimeout, allowPartialResults,
                                                                        collation, allowDiskUse);
ThorsAnvil_Template_MakeFilter(2, ThorsAnvil::DB::Mongo::Find,          findFilter);
ThorsAnvil_Template_ExpandTrait(2,ThorsAnvil::DB::Mongo::FindOptional,
                                  ThorsAnvil::DB::Mongo::Find,          find, filter, sort);

#include "CmdDB_Find.tpp"

#endif
