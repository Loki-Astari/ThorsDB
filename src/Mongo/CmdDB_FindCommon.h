#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_FIND_COMMON_H
#define THORS_ANVIL_DB_MONGO_CMD_DB_FIND_COMMON_H

#include "CmdDB_Reply.h"

namespace ThorsAnvil::DB::Mongo
{

// Template classes for Find/FindModify
struct FindAll      {};     // Used as the default Find   template parameter. Finds All.
struct DefaultSort  {};     // Used as the default Sort   template parameter. No sort applied.
struct NoUpdate     {};     // Used as the default Update template parameter. No update applied.

template<typename Document> struct CursorFirst;
template<typename Document> struct CursorNext;

template<typename T>
struct UserDataExtractor
{
    using UserData = T;
};

template<typename Document>
struct UserDataExtractor<CursorFirst<Document>>
{
    using UserData = typename CursorFirst<Document>::UserData;
};

template<typename Document>
struct UserDataExtractor<CursorNext<Document>>
{
    using UserData = typename CursorNext<Document>::UserData;
};

template<typename Document>
struct CursorFirst
{
    using UserData = std::vector<Document>;
    using Reference = std::reference_wrapper<UserData>;

    CursorFirst(UserData& container)
        : firstBatch(container)
    {}

    bool                    partialResultsReturned  = true;
    CursorId                id                      = 0;
    std::string             ns;
    Reference               firstBatch;
};

template<typename Document>
struct CursorNext
{
    using UserData = std::vector<Document>;
    using Reference = std::reference_wrapper<UserData>;

    CursorNext(UserData& container)
        : nextBatch(container)
    {}

    bool                    partialResultsReturned  = true;
    CursorId                id                      = 0;
    std::string             ns;
    Reference               nextBatch;
};

struct Signature
{
    std::int64_t            keyIdP                  = 0;
    std::string             hash;
};

struct ClusterTime
{
    std::time_t             clusterTime             = 0;
    Signature               signature;
};

template<typename Cursor>
struct FindReply: public CmdReplyBase
{
    using UserData  = typename UserDataExtractor<Cursor>::UserData;

    FindReply(UserData& container)
        : cursor(container)
    {}

    Cursor                  cursor;
    std::time_t             operationTime           = 0;
    ClusterTime             $clusterTime;
};

template<typename Cursor>
using CmdDB_FindReplyBase   = CmdDB_Reply<FindReply<Cursor>>;
}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::FindAll);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DefaultSort);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::NoUpdate);

// Result Info
ThorsAnvil_Template_MakeTrait(1,ThorsAnvil::DB::Mongo::CursorFirst,     partialResultsReturned, id, ns, firstBatch);
ThorsAnvil_Template_MakeTrait(1,ThorsAnvil::DB::Mongo::CursorNext,      partialResultsReturned, id, ns, nextBatch);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Signature,                  keyIdP, hash);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ClusterTime,                clusterTime, signature);
ThorsAnvil_Template_ExpandTrait(1, ThorsAnvil::DB::Mongo::CmdReplyBase,
                                ThorsAnvil::DB::Mongo::FindReply,       cursor, operationTime, $clusterTime);

#endif
