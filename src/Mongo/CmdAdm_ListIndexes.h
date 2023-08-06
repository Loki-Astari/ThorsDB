#ifndef THORS_ANVIL_DB_MONGO_CMD_ADM_LIST_INDEXES_H
#define THORS_ANVIL_DB_MONGO_CMD_ADM_LIST_INDEXES_H

// https://docs.mongodb.com/manual/reference/command/listIndexes/

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "CmdAdm_CreateIndex.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

// TODO Add ListIndexes Options.
// See CmdDB_Find for the pattern.

struct ListIndexes
{
    std::string    listIndexes;
};

struct ListIndexesReply: public CmdReplyBase
{
    using UserData = void;

    ListIndexesReply()
        : cursor(indexInfo)
    {}

    std::vector<Index>      indexInfo;
    CursorFirst<Index>      cursor;
};

using CmdAdm_ListIndexes      = CmdAdm_Query<ListIndexes>;
using CmdAdm_ListIndexesReply = CmdAdm_Reply<ListIndexesReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ListIndexes,      listIndexes);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                     ThorsAnvil::DB::Mongo::ListIndexesReply, cursor);

#endif
