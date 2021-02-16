#ifndef THORSANVIL_DB_MONGO_CMD_ADM_COMPACT_H
#define THORSANVIL_DB_MONGO_CMD_ADM_COMPACT_H

// https://docs.mongodb.com/manual/reference/command/compact/#compact

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

// TODO Add ListDataBase Options.
// See CmdDB_Find for the pattern.

struct Compact
{
    Compact(std::string compact)
        : compact(std::move(compact))
    {}
    std::string    compact;
};

using CmdAdm_Compact      = CmdAdm_Query<Compact>;
using CmdAdm_CompactReply = CmdAdm_Reply<CmdReplyBase>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Compact,            compact);

#endif
