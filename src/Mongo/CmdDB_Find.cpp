#include "CmdDB_Find.h"

using namespace ThorsAnvil::DB::Mongo;

FindOptional::FindOptional(FindOptions options)
    : projection(std::move(options.projection))
    , hint(std::move(options.hint))
    , skip(std::move(options.skip))
    , limit(std::move(options.limit))
    , batchSize(std::move(options.batchSize))
    , singleBatch(std::move(options.singleBatch))
    , comment(std::move(options.comment))
    , maxTimeMS(std::move(options.maxTimeMS))
    , readConcern(std::move(options.readConcern))
#pragma vera-pushoff
    , max(std::move(options.max))
    , min(std::move(options.min))
#pragma vera-pop
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

static const FindOptions defaultValue;


void FindOptional::updateFilter()
{
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

void FindOptional::addFileds(std::initializer_list<std::string> fieldNames)
{
    for (auto& name: fieldNames)
    {
        projection[std::move(name)] = 1;
    }
    optionsFilter["projection"]          = projection            != defaultValue.projection;
}

void FindOptional::addHint(std::string h)
{
    hint = std::move(h);
    optionsFilter["hint"]                = hint                  != defaultValue.hint;
}

void FindOptional::setSkip(std::size_t val)
{
    skip = val;
    optionsFilter["skip"]                = skip                  != defaultValue.skip;
}

void FindOptional::setLimit(std::size_t val)
{
    limit = val;
    optionsFilter["limit"]               = limit                 != defaultValue.limit;
}

void FindOptional::setBatchSize(std::size_t val)
{
    batchSize = val;
    optionsFilter["batchSize"]           = batchSize             != defaultValue.batchSize;
}

void FindOptional::oneBatch(bool val)
{
    // Already set true. Just need to return it.
    singleBatch = val;
    optionsFilter["singleBatch"]         = singleBatch           != defaultValue.singleBatch;
}

void FindOptional::setComment(std::string val)
{
    comment = std::move(val);
    optionsFilter["comment"]             = comment               != defaultValue.comment;
}

void FindOptional::setMaxTimeout(std::size_t val)
{
    maxTimeMS   = val;
    optionsFilter["maxTimeMS"]           = maxTimeMS             != defaultValue.maxTimeMS;
}

void FindOptional::addReadConcern(ReadConcernLevel val)
{
    readConcern = ReadConcern{std::move(val)};
    optionsFilter["readConcern"]         = readConcern           != defaultValue.readConcern;
}

void FindOptional::addMax(std::string field, int val)
{
    max[std::move(field)] = val;
    optionsFilter["max"]                 = max                   != defaultValue.max;
}

void FindOptional::addMin(std::string field, int val)
{
    min[std::move(field)] = val;
    optionsFilter["min"]                 = min                   != defaultValue.min;
}

void FindOptional::justKeys(bool val)
{
    // Already set true. Just need to return it.
    returnKey = !val;
    optionsFilter["returnKey"]           = returnKey             != defaultValue.returnKey;
}

void FindOptional::showId(bool val)
{
    // Already set true. Just need to return it.
    showRecordId = val;
    optionsFilter["showRecordId"]        = showRecordId          != defaultValue.showRecordId;
}

void FindOptional::tailableCursor(bool val)
{
    tailable = val;
    optionsFilter["tailable"]            = tailable              != defaultValue.tailable;
}

void FindOptional::tailedCursorAwait(bool val)
{
    awaitData = val;
    optionsFilter["tailable"]            = awaitData             != defaultValue.awaitData;
    optionsFilter["awaitData"]           = awaitData             != defaultValue.awaitData;
}

void FindOptional::setNoCursorTimeout(bool val)
{
    noCursorTimeout = val;
    optionsFilter["noCursorTimeout"]     = noCursorTimeout       != defaultValue.noCursorTimeout;
}

void FindOptional::setAllowPartialResults(bool val)
{
    allowPartialResults = val;
    optionsFilter["allowPartialResults"] = allowPartialResults   != defaultValue.allowPartialResults;
}

void FindOptional::useDisk(bool val)
{
    allowDiskUse = val;
    optionsFilter["allowDiskUse"]        = allowDiskUse          != defaultValue.allowDiskUse;
}
