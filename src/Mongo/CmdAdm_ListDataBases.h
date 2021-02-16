#ifndef THORSANVIL_DB_MONGO_CMD_ADM_LIST_DATABASES_H
#define THORSANVIL_DB_MONGO_CMD_ADM_LIST_DATABASES_H

// https://docs.mongodb.com/manual/reference/command/listCollections/#listcollections

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

struct ListDataBases
{
    bool    listDatabases = 1;
};

struct DBInfo
{
    std::string     name;
    double          sizeOnDisk;
    bool            empty;
};

struct ListDataBaseReply: public CmdReplyBase
{
    using UserData = void;

    std::vector<DBInfo> databases;
    double              totalSize;
};

using CmdAdm_ListDataBases      = CmdAdm_Query<ListDataBases>;
using CmdAdm_ListDataBasesReply = CmdAdm_Reply<ListDataBaseReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ListDataBases,      listDatabases);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DBInfo,             name, sizeOnDisk, empty);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                     ThorsAnvil::DB::Mongo::ListDataBaseReply,  databases, totalSize);

#endif
