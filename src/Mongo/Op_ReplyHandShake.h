#ifndef THORSANVIL_DB_MONGO_OP_REPLY_HAND_SHAKE_H
#define THORSANVIL_DB_MONGO_OP_REPLY_HAND_SHAKE_H

#include "Op_Reply.h"

namespace ThorsAnvil::DB::Mongo
{

struct HandShakeReplyDoc
{
    double          OK;
    int             code;
    std::string     errmsg;
    std::string     codeName;
};

using Op_ReplyHandShake = Op_Reply<HandShakeReplyDoc>;

}
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::HandShakeReplyDoc, OK, code, errmsg, codeName);

#endif
