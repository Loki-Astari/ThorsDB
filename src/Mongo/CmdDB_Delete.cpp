#include "CmdDB_Delete.h"

using namespace ThorsAnvil::DB::Mongo;

DeleteOptional::DeleteOptional(DeleteOptions const& options)
    : ordered(std::move(options.ordered))
    , writeConcern(std::move(options.writeConcern))
{
    updateFilter();
}

static const DeleteOptions defaultValue;

void DeleteOptional::updateFilter()
{
    filter["ordered"]       = ordered       != defaultValue.ordered;
    filter["writeConcern"]  = writeConcern  != defaultValue.writeConcern;
}

void DeleteOptional::unordered(bool val)
{
    ordered = !val;
    filter["ordered"]       = ordered       != defaultValue.ordered;
}

void DeleteOptional::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    writeConcern    = WriteConcern{w, j, wtimeout};
    filter["writeConcern"]  = writeConcern  != defaultValue.writeConcern;
}
