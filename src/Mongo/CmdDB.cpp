#include "CmdDB.h"

using namespace ThorsAnvil::DB::Mongo;

WriteConcern::WriteConcern(int w, bool j, std::time_t wtimeout)
    : w(w)
    , j(j)
    , wtimeout(wtimeout)
{}
