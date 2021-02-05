#ifndef THORSANVIL_DB_MONGO_CMD_DB_H
#define THORSANVIL_DB_MONGO_CMD_DB_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "Op_Query.h"
#include "Op_Reply.h"
#include "ThorSerialize/Traits.h"

#include <string>

namespace ThorsAnvil::DB::Mongo
{

enum class ReadConcernLevel {local, available, majority, linearizable};

struct Collation
{
    friend bool operator!=(Collation const& lhs, Collation const& rhs)
    {
        return std::tie(lhs.locale, lhs.caseLevel, lhs.caseFirst, lhs.strength, lhs.numericOrdering, lhs.alternate, lhs.maxVariable, lhs.backwards)
            != std::tie(rhs.locale, rhs.caseLevel, rhs.caseFirst, rhs.strength, rhs.numericOrdering, rhs.alternate, rhs.maxVariable, rhs.backwards);
    }
    std::string     locale;
    bool            caseLevel           = false;
    std::string     caseFirst;
    int             strength            = 0;
    bool            numericOrdering     = false;
    std::string     alternate;
    std::string     maxVariable;
    bool            backwards           = false;
};

struct WriteErrors
{
    std::size_t                 index;
    int                         code;
    std::string                 errmsg;
};

struct WriteConcernError
{
    int                         code;
    std::string                 errmsg;
};

struct WriteConcern
{
    WriteConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
    friend bool operator!=(WriteConcern const& lhs, WriteConcern const& rhs)
    {
        return std::tie(lhs.w, lhs.j, lhs.wtimeout) != std::tie(rhs.w, rhs.j, rhs.wtimeout);
    }

    int             w           = 0;
    bool            j           = 0;
    std::time_t     wtimeout    = 0;
};

}

ThorsAnvil_MakeEnum(ThorsAnvil::DB::Mongo::ReadConcernLevel,    local, available, majority, linearizable);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Collation,          locale, caseLevel, strength, numericOrdering, alternate, maxVariable, backwards);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcern,       w, j, wtimeout);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteErrors,        index, code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcernError,  code, errmsg);

#include "CmdDB.tpp"

#endif
