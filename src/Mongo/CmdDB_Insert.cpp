#include "CmdDB_Insert.h"

using namespace ThorsAnvil::DB::Mongo;

InsertOptional::InsertOptional(InsertOptions const& options)
    : ordered(options.ordered)
    , writeConcern(options.writeConcern)
    , bypassDocumentValidation(options.bypassDocumentValidation)
    , comment(options.comment)
{
    updateFilter();
}

InsertOptional::InsertOptional(InsertOptions&& options)
    : ordered(std::move(options.ordered))
    , writeConcern(std::move(options.writeConcern))
    , bypassDocumentValidation(std::move(options.bypassDocumentValidation))
    , comment(std::move(options.comment))
{
    updateFilter();
}

void InsertOptional::updateFilter()
{
    static const InsertOptions defaultValue;

    filter["ordered"]                  = ordered                    != defaultValue.ordered;;
    filter["writeConcern"]             = writeConcern               != defaultValue.writeConcern;
    filter["bypassDocumentValidation"] = bypassDocumentValidation   != defaultValue.bypassDocumentValidation;
    filter["comment"]                  = comment                    != defaultValue.comment;
}

void InsertOptional::unordered(bool val)
{
    ordered = !val;
    filter["ordered"] = true;
}

void InsertOptional::byPass(bool val)
{
    bypassDocumentValidation = val;
    filter["bypassDocumentValidation"] = true;
}

void InsertOptional::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    writeConcern    = WriteConcern{w, j, wtimeout};
    filter["writeConcern"]  = true;
}

void InsertOptional::setComment(std::string&& c)
{
    comment = c;
    filter["comment"] = true;
}
