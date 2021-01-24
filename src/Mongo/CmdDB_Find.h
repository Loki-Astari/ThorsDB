#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_H
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_H

// https://docs.mongodb.com/manual/reference/command/find/#dbcmd.find

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{

struct FindAll      {};
struct DefaultSort  {};

struct ReadConcern
{
    ReadConcernLevel    level;
};

class Sort {};

template<typename Filter, typename Sort>
class Find
{
    public:
        Find(std::string const& collection, std::size_t skip, std::size_t limit, std::size_t batchSize)
            : find(collection)
            , skip(skip)
            , limit(limit)
            , batchSize(batchSize)
        {
            outputFilter["skip"]        = skip  == 0 ? false : true;
            outputFilter["limit"]       = limit == 0 ? false : true;
            outputFilter["batchSize"]   = batchSize == 101 ? false : true;
        }
        Find(std::string const& collection, Filter&& filter, std::size_t skip, std::size_t limit, std::size_t batchSize)
            : find(collection)
            , filter(std::forward<Filter>(filter))
            , skip(skip)
            , limit(limit)
            , batchSize(batchSize)
        {
            outputFilter["filter"]      = true;
            outputFilter["skip"]        = skip  == 0 ? false : true;
            outputFilter["limit"]       = limit == 0 ? false : true;
            outputFilter["batchSize"]   = batchSize == 101 ? false : true;
        }
        Find(std::string const& collection, Filter&& filter, Sort&& sort, std::size_t skip, std::size_t limit, std::size_t batchSize)
            : find(collection)
            , filter(std::forward<Filter>(filter))
            , sort(std::forward<Sort>(sort))
            , skip(skip)
            , limit(limit)
            , batchSize(batchSize)
        {
            outputFilter["filter"]      = true;
            outputFilter["sort"]        = true;
            outputFilter["skip"]        = skip  == 0 ? false : true;
            outputFilter["limit"]       = limit == 0 ? false : true;
            outputFilter["batchSize"]   = batchSize == 101 ? false : true;
        }
        Find(std::string const& collection, Sort&& sort, std::size_t skip, std::size_t limit, std::size_t batchSize)
            : find(collection)
            , filter{}
            , sort(std::forward<Sort>(sort))
            , skip(skip)
            , limit(limit)
            , batchSize(batchSize)
        {
            outputFilter["sort"]        = true;
            outputFilter["skip"]        = skip  == 0 ? false : true;
            outputFilter["limit"]       = limit == 0 ? false : true;
            outputFilter["batchSize"]   = batchSize == 101 ? false : true;
        }
        void addFileds(std::initializer_list<std::string> const& fieldNames)
        {
            for (auto const& name: fieldNames)
            {
                projection[name] = 1;
            }
            outputFilter["projection"]  = true;
        }
        void addHint(std::string&& hint)
        {
            hint = std::forward<std::string>(hint);
            outputFilter["hint"]        = true;
        }
        void setSkip(std::size_t val)
        {
            skip = val;
            outputFilter["skip"]        = true;
        }
        void setLimit(std::size_t val)
        {
            limit = val;
            outputFilter["limit"]        = true;
        }
        void setBatchSize(std::size_t val)
        {
            batchSize = val;
            outputFilter["batchSize"]   = true;
        }
        void oneBatch()
        {
            // Already set true. Just need to return it.
            outputFilter["singleBatch"] = true;
        }
        void setComment(std::string&& val)
        {
            comment = std::forward<std::string>(val);
            outputFilter["comment"]     = true;
        }
        void setMaxTimeout(std::size_t val)
        {
            maxTimeMS   = val;
            outputFilter["maxTimeMS"]   = true;
        }
        void addReadConcern(ReadConcernLevel val)
        {
            readConcern = ReadConcern{val};
            outputFilter["readConcern"] = true;
        }
        void addMax(std::string const& field, int val)
        {
            max[field] = val;
            outputFilter["max"]         = true;
        }
        void addMin(std::string const& field, int val)
        {
            min[field] = val;
            outputFilter["min"]         = true;
        }
        void justKeys()
        {
            // Already set true. Just need to return it.
            outputFilter["returnKey"]   = true;
        }
        void showId()
        {
            // Already set true. Just need to return it.
            outputFilter["showRecordId"]= true;
        }
        void tailableCursor()
        {
            outputFilter["tailable"]    = true;
        }
        void tailedCursorAwait()
        {
            outputFilter["tailable"]    = true;
            outputFilter["awaitData"]   = true;
        }
        void setNoCursorTimeout()
        {
            outputFilter["noCursorTimeout"] = true;
        }
        void setAllowPartialResults()
        {
            outputFilter["allowPartialResults"] = true;
        }
        // TODO setCollation
        void useDisk()
        {
            outputFilter["allowDiskUse"]    = true;
        }
    private:
        friend class ThorsAnvil::Serialize::Traits<Find>;
        friend class ThorsAnvil::Serialize::Filter<Find>;
        std::map<std::string, bool>     outputFilter = {{"filter", false}, {"sort", false},
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

        std::string                 find;
        Filter                      filter;
        Sort                        sort;
        std::map<std::string, int>  projection;
        std::string                 hint;
        std::size_t                 skip;
        std::size_t                 limit;
        std::size_t                 batchSize;
        bool                        singleBatch         = true;
        std::string                 comment;
        std::size_t                 maxTimeMS;
        ReadConcern                 readConcern{ReadConcernLevel::local};
        std::map<std::string, int>  max;
        std::map<std::string, int>  min;
        bool                        returnKey           = true;
        bool                        showRecordId        = true;
        bool                        tailable            = true;
        bool                        awaitData           = true;
        bool                        noCursorTimeout     = true;
        bool                        allowPartialResults = true;
        Collation                   collation;
        bool                        allowDiskUse        = true;
};


template<typename Filter, typename Sort>
using CmdDB_Find      = CmdDB_Query<Find<Filter, Sort>>;

CmdDB_Find<FindAll, DefaultSort>
make_CmdDB_Find(std::string const& db, std::string const& collection, Op_QueryOptions const& options, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<FindAll, DefaultSort>(db, collection, options, skip, limit, batchSize);
}

template<typename Filter>
CmdDB_Find<Filter, DefaultSort>
make_CmdDB_Find(std::string const& db, std::string const& collection, Op_QueryOptions const& options, Filter&& filter, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<Filter, DefaultSort>(db, collection, options, std::forward<Filter>(filter), skip, limit, batchSize);
}

template<typename Filter, typename Sort>
CmdDB_Find<Filter, Sort>
make_CmdDB_Find(std::string const& db, std::string const& collection, Op_QueryOptions const& options, Filter&& filter, Sort&& sort, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<Filter, Sort>(db, collection, options, std::forward<Filter>(filter), std::forward<Sort>(sort), skip, limit, batchSize);
}

template<typename Sort>
CmdDB_Find<FindAll, Sort>
make_CmdDB_FindAllSorted(std::string const& db, std::string const& collection, Op_QueryOptions const& options, Sort&& sort, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<FindAll, Sort>(db, collection, options, std::forward<Sort>(sort), skip, limit, batchSize);
}

CmdDB_Find<FindAll, DefaultSort>
make_CmdDB_Find(std::string const& db, std::string const& collection, Op_QueryOptions&& options, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<FindAll, DefaultSort>(db, collection, std::move(options), skip, limit, batchSize);
}

template<typename Filter>
CmdDB_Find<Filter, DefaultSort>
make_CmdDB_Find(std::string const& db, std::string const& collection, Op_QueryOptions&& options, Filter&& filter, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<Filter, DefaultSort>(db, collection, std::move(options), std::forward<Filter>(filter), skip, limit, batchSize);
}

template<typename Filter, typename Sort>
CmdDB_Find<Filter, Sort>
make_CmdDB_Find(std::string const& db, std::string const& collection, Op_QueryOptions&& options, Filter&& filter, Sort&& sort, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<Filter, Sort>(db, collection, std::move(options), std::forward<Filter>(filter), std::forward<Sort>(sort), skip, limit, batchSize);
}

template<typename Sort>
CmdDB_Find<FindAll, Sort>
make_CmdDB_FindAllSorted(std::string const& db, std::string const& collection, Op_QueryOptions&& options, Sort&& sort, std::size_t skip = 0, std::size_t limit = 0, std::size_t batchSize = 101)
{
    return CmdDB_Find<FindAll, Sort>(db, collection, std::move(options), std::forward<Sort>(sort), skip, limit, batchSize);
}

}
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::FindAll);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DefaultSort);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ReadConcern,            level);
ThorsAnvil_Template_MakeFilter(2, ThorsAnvil::DB::Mongo::Find,      outputFilter);
ThorsAnvil_Template_MakeTrait(2, ThorsAnvil::DB::Mongo::Find,       find, filter, sort , projection, hint, skip, limit, batchSize,
                                                                    singleBatch, comment, maxTimeMS, readConcern, max, min, returnKey,
                                                                    showRecordId, tailable, awaitData, noCursorTimeout, allowPartialResults,
                                                                    collation, allowDiskUse);

#endif
