#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_H
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_H

// https://docs.mongodb.com/manual/reference/command/find/#dbcmd.find

#include "CmdDB.h"
#include "CmdDB_Query.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

struct FindAll      {};
struct DefaultSort  {};

struct ReadConcern
{
    friend bool operator!=(ReadConcern const& lhs, ReadConcern const& rhs)
    {
        return lhs.level != rhs.level;
    }
    ReadConcernLevel    level = ReadConcernLevel::local;
};

class Sort {};

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
        FindOptional(FindOptions options);

        void addFileds(std::initializer_list<std::string> const& fieldNames);
        void addHint(std::string const& hint);
        void setSkip(std::size_t val);
        void setLimit(std::size_t val);
        void setBatchSize(std::size_t val);
        void oneBatch(bool val = true);
        void setComment(std::string const& val);
        void setMaxTimeout(std::size_t val);
        void addReadConcern(ReadConcernLevel val);
        void addMax(std::string const& field, int val);
        void addMin(std::string const& field, int val);
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

        Find(FindOptions options, std::string collection, Filter&& filter, Sort&& sort);
    private:
        void updateFilter();
        friend class ThorsAnvil::Serialize::Traits<Find>;
        friend class ThorsAnvil::Serialize::Filter<Find>;

        std::string                 find;
        Filter                      filter;
        Sort                        sort;
        std::map<std::string, bool>     findFilter = {{"filter", false}, {"sort", false}};
};

template<typename Document>
struct Cursor
{
    bool                    partialResultsReturned  = true;
    CursorId                id                      = 0;
    std::string             ns;
    std::vector<Document>   firstBatch;
};

struct Signature
{
    std::int64_t            keyIdP                  = 0;
    std::string             hash;
};

struct ClusterTime
{
    std::time_t             clusterTime             = 0;
    Signature               signature;
};

template<typename Document>
struct FindResult
{
    Cursor<Document>        cursor;
    double                  ok                      = 0.0;
    std::time_t             operationTime           = 0;
    ClusterTime             $clusterTime;
};

template<typename Filter, typename Sort>
using CmdDB_Find        = CmdDB_Query<Find<Filter, Sort>>;

template<typename Document>
class CmdDB_FindResult: public Op_Reply<FindResult<Document>>
{
    public:
        FindResult<Document>    findData;
        CmdDB_FindResult()
            : Op_Reply<FindResult<Document>>(findData)
        {}
        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Reply<FindResult<Document>> const&>(*this));}
};

template<typename Filter = FindAll, typename Sort = DefaultSort>
CmdDB_Find<Filter, DefaultSort> make_CmdDB_Find(std::string db, std::string collection, FindOptions findOpt = {}, Filter&& filter = Filter{}, Sort&& sort = Sort{})
{
    return CmdDB_Find<Filter, DefaultSort>(std::move(db), std::move(collection), Op_QueryOptions{}, std::move(findOpt), std::forward<Filter>(filter), std::forward<Sort>(sort));
}

template<typename Filter = FindAll, typename Sort = DefaultSort>
CmdDB_Find<Filter, DefaultSort> make_CmdDB_Find(std::string db, std::string collection, Op_QueryOptions options, FindOptions findOpt = {}, Filter&& filter = Filter{}, Sort&& sort = Sort{})
{
    return CmdDB_Find<Filter, DefaultSort>(std::move(db), std::move(collection), std::move(options), std::move(findOpt), std::forward<Filter>(filter), std::forward<Sort>(sort));
}

template<typename Sort>
CmdDB_Find<FindAll, Sort> make_CmdDB_FindAllSorted(std::string db, std::string collection, FindOptions findOpt, Sort&& sort)
{
    return CmdDB_Find<FindAll, Sort>(std::move(db), std::move(collection), Op_QueryOptions{}, std::move(findOpt), FindAll{}, std::forward<Sort>(sort));
}

template<typename Sort>
CmdDB_Find<FindAll, Sort> make_CmdDB_FindAllSorted(std::string db, std::string collection, Op_QueryOptions options, FindOptions&& findOpt, Sort&& sort)
{
    return CmdDB_Find<FindAll, Sort>(std::move(db), std::move(collection), std::move(options), std::move(findOpt), FindAll{}, std::forward<Sort>(sort));
}

}

// Default Actions
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::FindAll);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DefaultSort);
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
// Result Info
ThorsAnvil_Template_MakeTrait(1,ThorsAnvil::DB::Mongo::Cursor,          partialResultsReturned, id, ns, firstBatch);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Signature,                  keyIdP, hash);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ClusterTime,                clusterTime, signature);
ThorsAnvil_Template_MakeTrait(1,ThorsAnvil::DB::Mongo::FindResult,      cursor, ok, operationTime, $clusterTime);

#include "CmdDB_Find.tpp"

#endif
