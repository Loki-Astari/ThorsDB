#include "CmdDB_Reply.h"

using namespace ThorsAnvil::DB::Mongo;

std::size_t CmdDB_Reply::replyCount() const
{
    // Assumes isCmdOK() is true
    return reply[0].n;
}

bool CmdDB_Reply::isOk() const
{
    return Op_Reply::isOk() && reply.size() > 0 && reply[0].ok == 1.0;
}

std::string CmdDB_Reply::getHRErrorMessage() const
{
    bool        ok     = true;
    std::string result = Op_Reply::getHRErrorMessage();
    result += ": CmdReply: ";

    if (reply[0].codeName.size() != 0 || reply[0].errmsg.size() != 0)
    {
        ok = false;
        result += std::to_string(reply[0].code);
        result += ": ";
        result += reply[0].codeName;
        result += ": ";
        result += reply[0].errmsg;
    }
    if (reply[0].writeErrors.size() != 0)
    {
        ok = false;
        result += ": writeErrors: ";
        for (auto const& error: reply[0].writeErrors)
        {
            result += " At: ";
            result += std::to_string(error.index);
            result += ": ";
            result += std::to_string(error.code);
            result += ": ";
            result += error.errmsg;
        }
    }
    if (ok)
    {
        result += "OK";
    }
    return result;
}
