#include "BaseCmd.h"

using namespace ThorsAnvil::DB::Mongo;

WriteConcern::WriteConcern(int w, bool j, std::time_t wtimeout)
    : w(w)
    , j(j)
    , wtimeout(wtimeout)
{}

bool CmdReplyBase::isOk() const
{
    return ok;
}

std::string CmdReplyBase::getHRErrorMessage() const
{
    std::stringstream result;

    if (codeName.size() != 0 || errmsg.size() != 0)
    {
        result  << std::to_string(code)
                << ": "
                << codeName
                << ": "
                << errmsg;
    }
    return result.str();
}

std::string CmdReply::getHRErrorMessage() const
{
    bool        ok     = true;
    std::stringstream result;
    result << ": CmdReply: ";

    std::string base = CmdReplyBase::getHRErrorMessage();
    if (base.size() != 0)
    {
        ok = false;
        result  << base;
    }
    if (writeErrors.size() != 0)
    {
        ok = false;
        result << ": writeErrors: ";
        for (auto const& error: writeErrors)
        {
            result  << " At: "
                    << std::to_string(error.index)
                    << ": "
                    << std::to_string(error.code)
                    << ": "
                    << error.errmsg;
        }
    }
    if (ok)
    {
        result << "OK";
    }
    return result.str();
}
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
