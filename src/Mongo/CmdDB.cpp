#include "CmdDB.h"

using namespace ThorsAnvil::DB::Mongo;

WriteConcern::WriteConcern(int w, bool j, std::time_t wtimeout)
    : w(w)
    , j(j)
    , wtimeout(wtimeout)
{}

namespace ThorsAnvil::DB::Mongo
{
    bool operator!=(Collation const& lhs, Collation const& rhs)
    {
        return std::tie(lhs.locale, lhs.caseLevel, lhs.caseFirst, lhs.strength, lhs.numericOrdering, lhs.alternate, lhs.maxVariable, lhs.backwards)
            != std::tie(rhs.locale, rhs.caseLevel, rhs.caseFirst, rhs.strength, rhs.numericOrdering, rhs.alternate, rhs.maxVariable, rhs.backwards);
    }

    bool operator!=(WriteConcern const& lhs, WriteConcern const& rhs)
    {
        return std::tie(lhs.w, lhs.j, lhs.wtimeout) != std::tie(rhs.w, rhs.j, rhs.wtimeout);
    }

    bool operator!=(ReadConcern const& lhs, ReadConcern const& rhs)
    {
        return lhs.level != rhs.level;
    }
}
