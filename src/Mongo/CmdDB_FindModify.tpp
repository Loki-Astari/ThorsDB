#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_FIND_MODIFY_H
#error  "This should only be included from CmdDB_FindModify.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

#pragma vera-pushoff
template<typename Document>
bool CmdDB_FindModifyReply<Document>::isOk() const
{
    return Op_Reply<FindModifyReply<Document>>::isOk() && findData.ok == 1.0;
}
#pragma vera-pop

}

#endif
