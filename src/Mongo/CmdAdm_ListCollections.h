#ifndef THORSANVIL_DB_MONGO_CMD_ADM_LIST_COLLECTIONS_H
#define THORSANVIL_DB_MONGO_CMD_ADM_LIST_COLLECTIONS_H

// https://docs.mongodb.com/manual/reference/command/listCollections/

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include <string>
#include <vector>

namespace ThorsAnvil::DB::Mongo
{

// TODO Add ListCollections Options.
// See CmdDB_Find for the pattern.

struct ListCollections
{
    bool    listCollections = 1;
};

struct CollectionInfo
{
};

struct CollectionIndex
{
    bool                readOnly;
    // uuid
};

struct CollectionData
{
    std::string         name;
    std::string         type;
    CollectionInfo      info;
    CollectionIndex     idIndex;
};

struct ListCollectionsReply: public CmdReplyBase
{
    using UserData = void;

    ListCollectionsReply()
        : cursor(collectionInfo)
    {}

    std::vector<CollectionData>     collectionInfo;
    CursorFirst<CollectionData>     cursor;
};

using CmdAdm_ListCollections      = CmdAdm_Query<ListCollections>;
using CmdAdm_ListCollectionsReply = CmdAdm_Reply<ListCollectionsReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ListCollections,        listCollections);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CollectionInfo);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CollectionIndex,        readOnly);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CollectionData,         name, type, info, idIndex);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                     ThorsAnvil::DB::Mongo::ListCollectionsReply,   cursor);


#endif
