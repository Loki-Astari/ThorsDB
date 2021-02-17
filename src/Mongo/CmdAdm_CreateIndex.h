#ifndef THORSANVIL_DB_MONGO_CMD_ADM_CREATE_INDEX_H
#define THORSANVIL_DB_MONGO_CMD_ADM_CREATE_INDEX_H

// https://docs.mongodb.com/manual/reference/command/createIndexes/

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

// TODO Add ListDataBase Options.
// See CmdDB_Find for the pattern.

struct Index
{
    Index()
    {}
    Index(std::string name, std::string key, bool asc = true)
        : key{{std::move(key), asc ? 1 : -1}}
        , name{std::move(name)}
    {}
    std::map<std::string, std::int32_t>     key;
    std::string                             name;
};

struct CreateIndex
{
    template<typename... Args>
    CreateIndex(std::string create, Args&&... args)
        : createIndexes(std::move(create))
        , indexes{std::forward<Args>(args)...}
    {}

    std::string         createIndexes;
    std::vector<Index>  indexes;
};

struct CreateIndexReply: public CmdReplyBase
{
    using UserData = void;

    bool                createdCollectionAutomatically      = false;
    std::int32_t        numIndexesBefore                    = 0;
    std::int32_t        numIndexesAfter                     = 0;
    std::string         note;
};

using CmdAdm_CreateIndex      = CmdAdm_Query<CreateIndex>;
using CmdAdm_CreateIndexReply = CmdAdm_Reply<CmdReplyBase>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Index,             key, name);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CreateIndex,       createIndexes, indexes);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                     ThorsAnvil::DB::Mongo::CreateIndexReply,  createdCollectionAutomatically, numIndexesBefore, numIndexesAfter, note);

#endif
