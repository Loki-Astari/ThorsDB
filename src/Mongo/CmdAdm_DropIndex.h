#ifndef THORS_ANVIL_DB_MONGO_CMD_ADM_DROP_INDEX_H
#define THORS_ANVIL_DB_MONGO_CMD_ADM_DROP_INDEX_H

// https://docs.mongodb.com/manual/reference/command/dropIndexes/

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

// TODO Add DropIndex Options.
// See CmdDB_Find for the pattern.

struct DropIndex
{
    template<typename... Args>
    DropIndex(std::string collection, Args&&... args)
        : dropIndexes(std::move(collection))
        , index{std::forward<Args>(args)...}
    {}

    std::string                 dropIndexes;
    std::vector<std::string>    index;
};

using CmdAdm_DropIndex      = CmdAdm_Query<DropIndex>;
using CmdAdm_DropIndexReply = CmdAdm_Reply<CmdReplyBase>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DropIndex,            dropIndexes, index);

#endif
