#include "CmdDB_FindModify.h"

using namespace ThorsAnvil::DB::Mongo;

static const FindModifyOptions defaultOptions;

FindModifyOptional::FindModifyOptional(FindModifyOptions options)
    : returnModified(options.returnModified)
    , fields(std::move(options.fields))
    , upsert(options.upsert)
    , bypassDocumentValidation(options.bypassDocumentValidation)
    , writeConcern(std::move(options.writeConcern))
    , collation(std::move(options.collation))
    //, arrayFilters(std::move(options.arrayFilters))
    , hint(std::move(options.hint))
    , comment(std::move(options.comment))
{
    filter["returnModified"]            = returnModified            != defaultOptions.returnModified;
    filter["fields"]                    = fields                    != defaultOptions.fields;
    filter["upsert"]                    = upsert                    != defaultOptions.upsert;
    filter["bypassDocumentValidation"]  = bypassDocumentValidation  != defaultOptions.bypassDocumentValidation;
    filter["writeConcern"]              = writeConcern              != defaultOptions.writeConcern;
    filter["collation"]                 = collation                 != defaultOptions.collation;
    // filter["arrayFilters"]              = arrayFilters              != defaultOptions.arrayFilters;
    filter["hint"]                      = hint                      != defaultOptions.hint;
    filter["comment"]                   = comment                   != defaultOptions.comment;
}

void FindModifyOptional::setNew(bool val)
{
    returnModified = val;
    filter["returnModified"]    = returnModified    != defaultOptions.returnModified;
}

void FindModifyOptional::addFileds(std::initializer_list<std::string> fieldNames)
{
    for (auto& name: fieldNames)
    {
        fields[std::move(name)] = 1;
    }
    filter["fields"]    = fields != defaultOptions.fields;
}

void FindModifyOptional::setUpsert(bool val)
{
    upsert = val;
    filter["upsert"]            = upsert            != defaultOptions.upsert;
}

void FindModifyOptional::byPass(bool val)
{
    bypassDocumentValidation = val;
    filter["bypassDocumentValidation"] = bypassDocumentValidation != defaultOptions.bypassDocumentValidation;
}

void FindModifyOptional::setWriteConcern(int w, bool j, std::time_t wtimeout)
{
    writeConcern = {w, j, wtimeout};
    filter["writeConcern"] = writeConcern != defaultOptions.writeConcern;
}

void FindModifyOptional::addHint(std::string val)
{
    hint = std::move(val);
    filter["hint"]              = hint              != defaultOptions.hint;
}

void FindModifyOptional::setComment(std::string val)
{
    comment = std::move(val);
    filter["comment"]           = comment           != defaultOptions.comment;
}
