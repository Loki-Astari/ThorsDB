#include "CmdDB_GetLastError.h"
#include "ThorSerialize/JsonThor.h"

using namespace ThorsAnvil::DB::Mongo;

static const GetLastErrorOptions defaultValues;

GetLastErrorOptional::GetLastErrorOptional(GetLastErrorOptions options)
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

GetLastError::GetLastError(GetLastErrorOptions options, std::string /*collection*/)
    : GetLastErrorOptional(std::move(options))
{
}

std::string GetLastErrorReply::getHRErrorMessage() const
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
