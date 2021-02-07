#ifndef THORSANVIL_DB_MONGO_CMD_DB_GET_LAST_ERROR_H
#define THORSANVIL_DB_MONGO_CMD_DB_GET_LAST_ERROR_H

// https://docs.mongodb.com/manual/reference/command/getLastError/#getlasterror

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
        GetLastErrorOptional(GetLastErrorOptions options);

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

        GetLastError(GetLastErrorOptions options, std::string collection);
    private:
        friend class ThorsAnvil::Serialize::Traits<GetLastError>;
        std::int32_t    getLastError    = 1;
};

struct Host
{
    std::string                     hostname;
    std::int32_t                    port;
};

struct GetLastErrorReply
{
    std::map<std::string, bool>     filter;
    double                          ok                  = 0.0;
    std::unique_ptr<std::string>    err;
    std::string                     ns;
    std::string                     index;
    std::string                     errmsg;
    std::int32_t                    code                = -1;
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
};

using CmdDB_GetLastError        = CmdDB_Query<GetLastError>;

class CmdDB_GetLastErrorReply: public Op_Reply<GetLastErrorReply>
{
    public:
        mutable GetLastErrorReply       lastReply;
        CmdDB_GetLastErrorReply();
        virtual bool        isOk()              const override;
        virtual std::string getHRErrorMessage() const override;
        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Reply<GetLastErrorReply> const&>(*this));}
};

inline
CmdDB_GetLastError make_CmdDB_GetLastError(std::string db, GetLastErrorOptions getLastErrorOpt = {})
{
    using namespace std::string_literals;
    return CmdDB_GetLastError(std::move(db), ""s, Op_QueryOptions{}, std::move(getLastErrorOpt));
}

inline
CmdDB_GetLastError make_CmdDB_GetLastError(std::string db, Op_QueryOptions options, GetLastErrorOptions getLastErrorOpt = {})
{
    using namespace std::string_literals;
    return CmdDB_GetLastError(std::move(db), ""s, std::move(options), std::move(getLastErrorOpt));
}

}

ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::GetLastErrorOptional,          filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::GetLastErrorOptional,           j, w, wtimeout, comment);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::GetLastErrorOptional,
                       ThorsAnvil::DB::Mongo::GetLastError,                 getLastError);

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Host,                           hostname, port);
ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::GetLastErrorReply,             filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::GetLastErrorReply,              ok, err, ns, index, errmsg, code, connectionId,
                                                                            lastOp, n, syncMillis, shards, singleShard, updatedExisting,
                                                                            upserted, wnote, wtimeout, waited, wtime, writtenTo);


#endif
