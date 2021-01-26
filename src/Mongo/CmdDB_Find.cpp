#include "CmdDB_Find.h"

using namespace ThorsAnvil::DB::Mongo;

FindOptional::FindOptional(FindOptions const& options)
    : projection(std::move(options.projection))
    , hint(options.hint)
    , skip(options.skip)
    , limit(options.limit)
    , batchSize(options.batchSize)
    , singleBatch(options.singleBatch)
    , comment(options.comment)
    , maxTimeMS(options.maxTimeMS)
    , readConcern(options.readConcern)
    , max(options.max)
    , min(options.min)
    , returnKey(options.returnKey)
    , showRecordId(options.showRecordId)
    , tailable(options.tailable)
    , awaitData(options.awaitData)
    , noCursorTimeout(options.noCursorTimeout)
    , allowPartialResults(options.allowPartialResults)
    , collation(options.collation)
    , allowDiskUse(options.allowDiskUse)
{
    updateFilter();
}

FindOptional::FindOptional(FindOptions&& options)
    : projection(std::move(options.projection))
    , hint(std::move(options.hint))
    , skip(std::move(options.skip))
    , limit(std::move(options.limit))
    , batchSize(std::move(options.batchSize))
    , singleBatch(std::move(options.singleBatch))
    , comment(std::move(options.comment))
    , maxTimeMS(std::move(options.maxTimeMS))
    , readConcern(std::move(options.readConcern))
    , max(std::move(options.max))
    , min(std::move(options.min))
    , returnKey(std::move(options.returnKey))
    , showRecordId(std::move(options.showRecordId))
    , tailable(std::move(options.tailable))
    , awaitData(std::move(options.awaitData))
    , noCursorTimeout(std::move(options.noCursorTimeout))
    , allowPartialResults(std::move(options.allowPartialResults))
    , collation(std::move(options.collation))
    , allowDiskUse(std::move(options.allowDiskUse))
{
    updateFilter();
}

void FindOptional::updateFilter()
{
    static const FindOptions defaultValue;

    optionsFilter["projection"]          = projection            != defaultValue.projection;
    optionsFilter["hint"]                = hint                  != defaultValue.hint;
    optionsFilter["skip"]                = skip                  != defaultValue.skip;
    optionsFilter["limit"]               = limit                 != defaultValue.limit;
    optionsFilter["batchSize"]           = batchSize             != defaultValue.batchSize;
    optionsFilter["singleBatch"]         = singleBatch           != defaultValue.singleBatch;
    optionsFilter["comment"]             = comment               != defaultValue.comment;
    optionsFilter["maxTimeMS"]           = maxTimeMS             != defaultValue.maxTimeMS;
    optionsFilter["readConcern"]         = readConcern           != defaultValue.readConcern;
    optionsFilter["max"]                 = max                   != defaultValue.max;
    optionsFilter["min"]                 = min                   != defaultValue.min;
    optionsFilter["returnKey"]           = returnKey             != defaultValue.returnKey;
    optionsFilter["showRecordId"]        = showRecordId          != defaultValue.showRecordId;
    optionsFilter["tailable"]            = tailable              != defaultValue.tailable;
    optionsFilter["awaitData"]           = awaitData             != defaultValue.awaitData;
    optionsFilter["noCursorTimeout"]     = noCursorTimeout       != defaultValue.noCursorTimeout;
    optionsFilter["allowPartialResults"] = allowPartialResults   != defaultValue.allowPartialResults;
    optionsFilter["collation"]           = collation             != defaultValue.collation;
    optionsFilter["allowDiskUse"]        = allowDiskUse          != defaultValue.allowDiskUse;
}

void FindOptional::addFileds(std::initializer_list<std::string> const& fieldNames)
{
    for (auto const& name: fieldNames)
    {
        projection[name] = 1;
    }
    optionsFilter["projection"]  = true;
}

void FindOptional::addHint(std::string&& hint)
{
    hint = std::forward<std::string>(hint);
    optionsFilter["hint"]        = true;
}

void FindOptional::setSkip(std::size_t val)
{
    skip = val;
    optionsFilter["skip"]        = true;
}

void FindOptional::setLimit(std::size_t val)
{
    limit = val;
    optionsFilter["limit"]        = true;
}

void FindOptional::setBatchSize(std::size_t val)
{
    batchSize = val;
    optionsFilter["batchSize"]   = true;
}

void FindOptional::oneBatch()
{
    // Already set true. Just need to return it.
    optionsFilter["singleBatch"] = true;
}

void FindOptional::setComment(std::string&& val)
{
    comment = std::forward<std::string>(val);
    optionsFilter["comment"]     = true;
}

void FindOptional::setMaxTimeout(std::size_t val)
{
    maxTimeMS   = val;
    optionsFilter["maxTimeMS"]   = true;
}

void FindOptional::addReadConcern(ReadConcernLevel val)
{
    readConcern = ReadConcern{val};
    optionsFilter["readConcern"] = true;
}

void FindOptional::addMax(std::string const& field, int val)
{
    max[field] = val;
    optionsFilter["max"]         = true;
}

void FindOptional::addMin(std::string const& field, int val)
{
    min[field] = val;
    optionsFilter["min"]         = true;
}

void FindOptional::justKeys()
{
    // Already set true. Just need to return it.
    optionsFilter["returnKey"]   = true;
}

void FindOptional::showId()
{
    // Already set true. Just need to return it.
    optionsFilter["showRecordId"]= true;
}

void FindOptional::tailableCursor()
{
    optionsFilter["tailable"]    = true;
}

void FindOptional::tailedCursorAwait()
{
    optionsFilter["tailable"]    = true;
    optionsFilter["awaitData"]   = true;
}

void FindOptional::setNoCursorTimeout()
{
    optionsFilter["noCursorTimeout"] = true;
}

void FindOptional::setAllowPartialResults()
{
    optionsFilter["allowPartialResults"] = true;
}

void FindOptional::useDisk()
{
    optionsFilter["allowDiskUse"]    = true;
}
