#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_COMMON_H
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_COMMON_H

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
struct DocumentExtractor
{
    using Doc = T;
};

template<typename Document>
struct DocumentExtractor<CursorFirst<Document>>
{
    using Doc = Document;
};

template<typename Document>
struct DocumentExtractor<CursorNext<Document>>
{
    using Doc = Document;
};

template<typename Document>
struct CursorFirst
{
    CursorFirst(std::vector<Document>& container)
        : firstBatch(container)
    {}

    bool                    partialResultsReturned  = true;
    CursorId                id                      = 0;
    std::string             ns;
    std::reference_wrapper<std::vector<Document>>  firstBatch;
};

template<typename Document>
struct CursorNext
{
    CursorNext(std::vector<Document>& container)
        : nextBatch(container)
    {}

    bool                    partialResultsReturned  = true;
    CursorId                id                      = 0;
    std::string             ns;
    std::reference_wrapper<std::vector<Document>>  nextBatch;
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
struct FindReply
{
    using Document = typename DocumentExtractor<Cursor>::Doc;
    using Options  = Document;
    FindReply(std::vector<Options>& container)
        : cursor(container)
    {}

    Cursor                  cursor;
    double                  ok                      = 0.0;
    std::time_t             operationTime           = 0;
    ClusterTime             $clusterTime;

    bool isOk() const                       {return ok;}
    std::string getHRErrorMessage() const   {return "XX";}
};

template<typename Cursor>
using CmdDB_FindReplyBase   = CmdDB_Reply<FindReply<Cursor>>;
}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::FindAll);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DefaultSort);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::NoUpdate);

#endif
