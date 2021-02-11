#include "CmdDB_GetMore.h"

using namespace ThorsAnvil::DB::Mongo;

static const GetMoreOptions defaultValue;

GetMoreOptional::GetMoreOptional(GetMoreOptions const& options)
    : batchSize(options.batchSize)
    , maxTimeMS(options.maxTimeMS)
    , comment(std::move(options.comment))
{
    filter["batchSize"]     = batchSize != defaultValue.batchSize;
    filter["maxTimeMS"]     = maxTimeMS != defaultValue.maxTimeMS;
    filter["comment"]       = comment   != defaultValue.comment;
}

void GetMoreOptional::setBatchSize(std::size_t val)
{
    batchSize = val;
    filter["batchSize"]     = batchSize != defaultValue.batchSize;
}

void GetMoreOptional::setMaxTimeout(std::size_t val)
{
    maxTimeMS = val;
    filter["maxTimeMS"]     = maxTimeMS != defaultValue.maxTimeMS;
}

void GetMoreOptional::setComment(std::string c)
{
    comment = std::move(c);
    filter["comment"]       = comment != defaultValue.comment;
}

GetMore::GetMore(GetMoreOptions const& options, std::string collection, CursorId cursor)
    : GetMoreOptional(options)
    , getMore(cursor)
    , collection(std::move(collection))
{}
