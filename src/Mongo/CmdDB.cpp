#include "CmdDB.h"

using namespace ThorsAnvil::DB::Mongo;

bool CmdDB_Reply::replyCount() const
{
    // Assumes isCmdOK() is true
    return getDocument(0).n;
}

bool CmdDB_Reply::isOk() const
{
    return Op_Reply::isOk() && getDocumentCount() > 0 && getDocument(0).ok == 1.0;
}

std::string CmdDB_Reply::getHRErrorMessage() const
{
    bool        ok     = true;
    std::string result = Op_Reply::getHRErrorMessage();
    result += ": CmdReply: ";
    CmdReply const& reply = getDocument(0);
    if (reply.codeName.size() != 0 || reply.errmsg.size() != 0)
    {
        ok = false;
        result += std::to_string(reply.code);
        result += ": ";
        result += reply.codeName;
        result += ": ";
        result += reply.errmsg;
    }
    if (reply.writeErrors.size() != 0)
    {
        ok = false;
        result += ": writeErrors: ";
        for (auto const& error: reply.writeErrors)
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

WriteConcern::WriteConcern(int w, bool j, std::time_t wtimeout)
    : w(w)
    , j(j)
    , wtimeout(wtimeout)
{}
