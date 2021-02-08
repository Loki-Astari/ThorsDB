#include "CmdDB_Reply.h"

using namespace ThorsAnvil::DB::Mongo;

bool CmdReply::isOk() const
{
    return ok;
}

std::string CmdReply::getHRErrorMessage() const
{
    bool        ok     = true;
    std::stringstream result;
    result << ": CmdReply: ";

    if (codeName.size() != 0 || errmsg.size() != 0)
    {
        ok = false;
        result  << std::to_string(code)
                << ": "
                << codeName
                << ": "
                << errmsg;
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
