#ifndef THORSANVIL_DB_MONGO_OP_REPLY_HAND_SHAKE_H
#define THORSANVIL_DB_MONGO_OP_REPLY_HAND_SHAKE_H

#include "Op_Reply.h"
#include "ThorSerialize/MongoUtility.h"

namespace ThorsAnvil::DB::Mongo
{

using ObjectID      = ThorsAnvil::Serialize::MongoUtility::ObjectID;
using UTCDateTime   = ThorsAnvil::Serialize::MongoUtility::UTCDateTime;

struct Version
{
    ThorsAnvil::Serialize::MongoUtility::ObjectID            processId;
    std::int64_t        counter;
};
struct HandShakeReplyDoc
{
    double          ok;
    // If ok is zero
    int             code;
    std::string     errmsg;
    std::string     codeName;
    // If ok is 00 00 00 00 00 00 f0 3f
    Version         topologyVersion;
    ThorsAnvil::Serialize::MongoUtility::UTCDateTime     localTime;
    std::int32_t    maxBsonObjectSize;
    std::int32_t    maxMessageSizeBytes;
    std::int32_t    maxWriteBatchSize;
    std::int32_t    logicalSessionTimeoutMinutes;
    std::int32_t    connectionId;
    std::int32_t    minWireVersion;
    std::int32_t    maxWireVersion;
    bool            ismaster;
    bool            readOnly;
    std::vector<std::string>    saslSupportedMechs;
};

using Op_ReplyHandShake = Op_Reply<HandShakeReplyDoc>;

#if 0
#endif

}
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Version, processId, counter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::HandShakeReplyDoc, ok, code, errmsg, codeName, topologyVersion, localTime, maxBsonObjectSize, maxMessageSizeBytes, maxWriteBatchSize, logicalSessionTimeoutMinutes, connectionId, minWireVersion, maxWireVersion, ismaster, readOnly, saslSupportedMechs);

#endif
