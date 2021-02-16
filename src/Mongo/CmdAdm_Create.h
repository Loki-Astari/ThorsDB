#ifndef THORSANVIL_DB_MONGO_CMD_ADM_CREATE_H
#define THORSANVIL_DB_MONGO_CMD_ADM_CREATE_H

// https://docs.mongodb.com/manual/reference/command/compact/#compact

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

// TODO Add ListDataBase Options.
// See CmdDB_Find for the pattern.

struct Create
{
    Create(std::string create)
        : create(std::move(create))
    {}
    std::string    create;
};

using CmdAdm_Create      = CmdAdm_Query<Create>;
using CmdAdm_CreateReply = CmdAdm_Reply<CmdReplyBase>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Create,            create);

#endif
