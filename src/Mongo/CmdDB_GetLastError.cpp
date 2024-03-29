#include "CmdDB_GetLastError.h"
#include "ThorSerialize/JsonThor.h"

using namespace ThorsAnvil::DB::Mongo;

static const GetLastErrorOptions defaultValues;

GetLastErrorOptional::GetLastErrorOptional(GetLastErrorOptions const& options)
    : j(options.j)
    , w(options.w)
    , wtimeout(options.wtimeout)
    , comment(std::move(options.comment))
{
    filter["j"]                 = j         != defaultValues.j;
    filter["w"]                 = w         != defaultValues.w;
    filter["wtimeout"]          = wtimeout  != defaultValues.wtimeout;
    filter["comment"]           = comment   != defaultValues.comment;
}

void GetLastErrorOptional::waitFoolDiskFlush(bool val)
{
    j = !val;
    filter["j"]                 = j         != defaultValues.j;
}

void GetLastErrorOptional::waitForReplication(std::int32_t count)
{
    w = count;
    filter["w"]                 = w         != defaultValues.w;
    filter["wtimeout"]          = w         != defaultValues.w;
}

void GetLastErrorOptional::setWaitTimeout(std::int32_t millisec)
{
    wtimeout    = millisec;
    filter["wtimeout"]          = wtimeout  != defaultValues.wtimeout;
}

void GetLastErrorOptional::setComment(std::string c)
{
    comment     = std::move(c);
    filter["comment"]           = comment   != defaultValues.comment;
}

GetLastError::GetLastError(GetLastErrorOptions const& options, std::string /*collection*/)
    : GetLastErrorOptional(options)
{
}

std::string GetLastErrorReply::to_String() const
{
    static const GetLastErrorReply defaultValue;

    filter["err"]               = err               != defaultValue.err;
    filter["ns"]                = ns                != defaultValue.ns;
    filter["index"]             = index             != defaultValue.index;
    filter["errmsg"]            = errmsg            != defaultValue.errmsg;
    filter["code"]              = code              != defaultValue.code;
    filter["connectionId"]      = connectionId      != defaultValue.connectionId;
    filter["lastOp"]            = lastOp            != defaultValue.lastOp;
    filter["n"]                 = n                 != defaultValue.n;
    filter["syncMillis"]        = syncMillis        != defaultValue.syncMillis;
    filter["shards"]            = shards            != defaultValue.shards;
    filter["singleShard"]       = singleShard       != defaultValue.singleShard;
    filter["updatedExisting"]   = updatedExisting   != defaultValue.updatedExisting;
    filter["upserted"]          = upserted          != defaultValue.upserted;
    filter["wnote"]             = wnote             != defaultValue.wnote;
    filter["wtimeout"]          = wtimeout          != defaultValue.wtimeout;
    filter["waited"]            = waited            != defaultValue.waited;
    filter["wtime"]             = wtime             != defaultValue.wtime;
    filter["writtenTo"]         = writtenTo.get() != nullptr && writtenTo->size() != 0;

    std::stringstream   errorMsg;
    errorMsg << ThorsAnvil::Serialize::jsonExporter(*this);

    return errorMsg.str();
}

namespace ThorsAnvil::DB::Mongo
{

CmdDB_GetLastError send_CmdDB_GetLastError(std::string db, GetLastErrorOptions const& getLastErrorOpt)
{
    using namespace std::string_literals;
    return CmdDB_GetLastError(std::move(db), ""s, {}, getLastErrorOpt);
}

CmdDB_GetLastError send_CmdDB_GetLastError(std::string db, Op_QueryOptions const& options, GetLastErrorOptions const& getLastErrorOpt)
{
    using namespace std::string_literals;
    return CmdDB_GetLastError(std::move(db), ""s, options, getLastErrorOpt);
}

}
