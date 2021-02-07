#include "CmdDB_GetLastError.h"
#include "ThorSerialize/JsonThor.h"

using namespace ThorsAnvil::DB::Mongo;

static const GetLastErrorOptions defaultValues;

GetLastErrorOptional::GetLastErrorOptional(GetLastErrorOptions options)
    : j(options.j)
    , w(options.w)
    , wtimeout(options.w)
    , comment(std::move(options.comment))
{
    filter["j"]                 = j         == defaultValues.j;
    filter["w"]                 = w         == defaultValues.w;
    filter["wtimeout"]          = wtimeout  == defaultValues.wtimeout;
    filter["comment"]           = comment   == defaultValues.comment;
}

void GetLastErrorOptional::waitFoolDiskFlush(bool val)
{
    j = !val;
    filter["j"] = true;
}

void GetLastErrorOptional::waitForReplication(std::int32_t count)
{
    w = count;
    filter["w"]         = true;
    filter["wtimeout"]  = true;
}

void GetLastErrorOptional::setWaitTimeout(std::int32_t millisec)
{
    wtimeout    = millisec;
    filter["wtimeout"]  = true;
}

void GetLastErrorOptional::setComment(std::string c)
{
    comment     = std::move(c);
    filter["comment"]   = true;
}

GetLastError::GetLastError(GetLastErrorOptions options, std::string /*collection*/)
    : GetLastErrorOptional(std::move(options))
{
}

CmdDB_GetLastErrorReply::CmdDB_GetLastErrorReply()
    : Op_Reply<GetLastErrorReply>(lastReply)
{}

bool CmdDB_GetLastErrorReply::isOk() const
{
    return Op_Reply<GetLastErrorReply>::isOk() && lastReply.ok == 1.0;
}

std::string CmdDB_GetLastErrorReply::getHRErrorMessage() const
{
    static const GetLastErrorReply defaultValue;

    lastReply.filter["err"]               = lastReply.err               != defaultValue.err;
    lastReply.filter["ns"]                = lastReply.ns                != defaultValue.ns;
    lastReply.filter["index"]             = lastReply.index             != defaultValue.index;
    lastReply.filter["errmsg"]            = lastReply.errmsg            != defaultValue.errmsg;
    lastReply.filter["code"]              = lastReply.code              != defaultValue.code;
    lastReply.filter["connectionId"]      = lastReply.connectionId      != defaultValue.connectionId;
    lastReply.filter["lastOp"]            = lastReply.lastOp            != defaultValue.lastOp;
    lastReply.filter["n"]                 = lastReply.n                 != defaultValue.n;
    lastReply.filter["syncMillis"]        = lastReply.syncMillis        != defaultValue.syncMillis;
    lastReply.filter["shards"]            = lastReply.shards            != defaultValue.shards;
    lastReply.filter["singleShard"]       = lastReply.singleShard       != defaultValue.singleShard;
    lastReply.filter["updatedExisting"]   = lastReply.updatedExisting   != defaultValue.updatedExisting;
    lastReply.filter["upserted"]          = lastReply.upserted          != defaultValue.upserted;
    lastReply.filter["wnote"]             = lastReply.wnote             != defaultValue.wnote;
    lastReply.filter["wtimeout"]          = lastReply.wtimeout          != defaultValue.wtimeout;
    lastReply.filter["waited"]            = lastReply.waited            != defaultValue.waited;
    lastReply.filter["wtime"]             = lastReply.wtime             != defaultValue.wtime;
    lastReply.filter["writtenTo"]         = lastReply.writtenTo.get() != nullptr && lastReply.writtenTo->size() != 0;

    std::stringstream   errorMsg;
    errorMsg << Op_Reply<GetLastErrorReply>::getHRErrorMessage()
             << ThorsAnvil::Serialize::jsonExporter(lastReply);

    return errorMsg.str();
}
