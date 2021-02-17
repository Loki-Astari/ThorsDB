#ifndef THORSANVIL_DB_MONGO_CMD_ADM_DROP_H
#define THORSANVIL_DB_MONGO_CMD_ADM_DROP_H

// https://docs.mongodb.com/manual/reference/command/drop/

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

// TODO Add ListDataBase Options.
// See CmdDB_Find for the pattern.

struct Drop
{
    std::string    drop;
};

using CmdAdm_Drop        = CmdAdm_Query<Drop>;
using CmdAdm_DropReply   = CmdAdm_Reply<CmdReplyBase>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Drop,            drop);

#endif
