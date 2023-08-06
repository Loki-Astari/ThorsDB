#ifndef THORS_ANVIL_DB_MONGO_CMD_ADM_LOGOUT_H
#define THORS_ANVIL_DB_MONGO_CMD_ADM_LOGOUT_H

// https://docs.mongodb.com/manual/reference/command/logout/

#include "CmdAdm_Query.h"
#include "CmdAdm_Reply.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

struct Logout
{
    std::int32_t        logout = 1;
};

using CmdAdm_Logout        = CmdAdm_Query<Logout>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Logout,            logout);

#endif
