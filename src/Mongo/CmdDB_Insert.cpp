#include "CmdDB_Insert.h"

using namespace ThorsAnvil::DB::Mongo;

InsertOptional::InsertOptional(InsertOptions const& options)
    : ordered(std::move(options.ordered))
    , writeConcern(std::move(options.writeConcern))
    , bypassDocumentValidation(std::move(options.bypassDocumentValidation))
    , comment(std::move(options.comment))
{
    updateFilter();
}

static const InsertOptions defaultValue;

void InsertOptional::updateFilter()
{
    filter["ordered"]                  = ordered                    != defaultValue.ordered;;
    filter["writeConcern"]             = writeConcern               != defaultValue.writeConcern;
    filter["bypassDocumentValidation"] = bypassDocumentValidation   != defaultValue.bypassDocumentValidation;
    filter["comment"]                  = comment                    != defaultValue.comment;
}

void InsertOptional::unordered(bool val)
{
    ordered = !val;
    filter["ordered"]                  = ordered                    != defaultValue.ordered;
}

void InsertOptional::byPass(bool val)
{
    bypassDocumentValidation = val;
    filter["bypassDocumentValidation"] = bypassDocumentValidation   != defaultValue.bypassDocumentValidation;
}

void InsertOptional::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    writeConcern    = WriteConcern{w, j, wtimeout};
    filter["writeConcern"]              = writeConcern              != defaultValue.writeConcern;
}

void InsertOptional::setComment(std::string c)
{
    comment = std::move(c);
    filter["comment"]                   = comment                   != defaultValue.comment;
}
