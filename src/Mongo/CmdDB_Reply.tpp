#ifndef THORSANVIL_DB_MONGO_CMD_DB_REPLY_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_REPLY_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_REPLY_H
#error  "This should only be included from CmdDB_Reply.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
bool CmdDB_Reply<Document>::isOk() const
{
    return Op_Reply<ViewType<Document>>::isOk() && reply.isOk();
}

template<typename Document>
std::string CmdDB_Reply<Document>::getHRErrorMessage() const
{
    return Op_Reply<ViewType<Document>>::getHRErrorMessage() + reply.getHRErrorMessage();
}

}

#endif
