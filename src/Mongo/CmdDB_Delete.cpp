#include "CmdDB_Delete.h"

using namespace ThorsAnvil::DB::Mongo;

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
