#ifndef THORSANVIL_DB_MONGO_CMD_DB_GET_LAST_ERROR_H
#define THORSANVIL_DB_MONGO_CMD_DB_GET_LAST_ERROR_H

// https://docs.mongodb.com/manual/reference/command/getLastError/#getlasterror

/* $    Usage: CmdDB_GetLastError
 * $        connection:         connection to mongo DB or a stream.
 * $        Op_QueryOptions:    See: Op_Query.h
 * $        GetLastErrorOptions:See: below
 *
 * >    connection << send_CmdDB_GetLastError("db", [, Op_Query_Options] [, GetLastErrorOptions]);
 * >    CmdDB_GetLastErrorReply     lastError
 * >    connection >> lastError;
 * >    std::cout << lastError.getHRErrorMessage();
 */
#include "CmdDB.h"
#include "CmdDB_Query.h"
#include "CmdDB_Reply.h"

#include <map>
#include <string>

namespace ThorsAnvil::DB::Mongo
{

struct GetLastErrorOptions
{
    bool            j           = true;
    std::int32_t    w           = 1;            // Needs to support "majority"
    std::int32_t    wtimeout    = 0;
    std::string     comment;
};

struct GetLastErrorOptional
{
    public:
        GetLastErrorOptional(GetLastErrorOptions const& options);

        void waitFoolDiskFlush(bool val = true);
        void waitForReplication(std::int32_t count);
        void setWaitTimeout(std::int32_t millisec);
        void setComment(std::string comment);
    private:
        friend class ThorsAnvil::Serialize::Traits<GetLastErrorOptional>;
        friend class ThorsAnvil::Serialize::Filter<GetLastErrorOptional>;
        std::map<std::string, bool>     filter;

        bool            j;
        std::int32_t    w;
        std::int32_t    wtimeout;
        std::string     comment;
};

class GetLastError: public GetLastErrorOptional
{
    public:
        using Options = GetLastErrorOptions;

        GetLastError(GetLastErrorOptions const& options, std::string collection);
    private:
        friend class ThorsAnvil::Serialize::Traits<GetLastError>;
        std::int32_t    getLastError    = 1;
};

struct Host
{
    std::string                     hostname;
    std::int32_t                    port;
};

struct GetLastErrorReply: public CmdReplyBase
{
    using UserData = void;

    mutable std::map<std::string, bool>     filter;

    std::unique_ptr<std::string>    err;
    std::string                     ns;
    std::string                     index;
    std::int32_t                    connectionId        = -1;   // *
    std::time_t                     lastOp              = 0;
    std::int32_t                    n                   = -1;   // *
    std::int32_t                    syncMillis          = -1;
    std::int32_t                    shards              = -1;
    std::int32_t                    singleShard         = -1;
    bool                            updatedExisting     = false;
    bool                            upserted            = false;
    std::string                     wnote;
    bool                            wtimeout            = false;
    std::time_t                     waited              = 0;
    std::time_t                     wtime               = 0;
    std::unique_ptr<std::vector<Host>> writtenTo;                  // *

    std::string to_String() const;
};

using CmdDB_GetLastError        = CmdDB_Query<GetLastError>;

using CmdDB_GetLastErrorReply   = CmdDB_Reply<GetLastErrorReply>;

CmdDB_GetLastError send_CmdDB_GetLastError(std::string db, GetLastErrorOptions const& getLastErrorOpt = {});

CmdDB_GetLastError send_CmdDB_GetLastError(std::string db, Op_QueryOptions const& options, GetLastErrorOptions const& getLastErrorOpt = {});

}

ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::GetLastErrorOptional,          filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::GetLastErrorOptional,           j, w, wtimeout, comment);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::GetLastErrorOptional,
                       ThorsAnvil::DB::Mongo::GetLastError,                 getLastError);

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Host,                           hostname, port);
ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::GetLastErrorReply,             filter);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                     ThorsAnvil::DB::Mongo::GetLastErrorReply,              err, ns, index, connectionId,
                                                                            lastOp, n, syncMillis, shards, singleShard, updatedExisting,
                                                                            upserted, wnote, wtimeout, waited, wtime, writtenTo);


#endif
