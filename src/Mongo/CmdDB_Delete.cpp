#include "CmdDB_Delete.h"

using namespace ThorsAnvil::DB::Mongo;

DeleteOptional::DeleteOptional(DeleteOptions options)
    : ordered(std::move(options.ordered))
    , writeConcern(std::move(options.writeConcern))
{
    updateFilter();
}

void DeleteOptional::updateFilter()
{
    static const DeleteOptions  defaultValue;

    filter["ordered"]       = ordered       != defaultValue.ordered;
    filter["writeConcern"]  = writeConcern  != defaultValue.writeConcern;
}

void DeleteOptional::unordered(bool val)
{
    ordered = !val;
    filter["ordered"] = true;
}

void DeleteOptional::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    writeConcern    = WriteConcern{w, j, wtimeout};
    filter["writeConcern"]  = true;
}
