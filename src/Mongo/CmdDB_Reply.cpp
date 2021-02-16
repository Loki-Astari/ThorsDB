#include "CmdDB_Reply.h"

using namespace ThorsAnvil::DB::Mongo;

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
