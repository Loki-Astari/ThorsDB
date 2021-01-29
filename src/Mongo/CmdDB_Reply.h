#ifndef THORSANVIL_DB_MONGO_CMD_DB_REPLY_H
#define THORSANVIL_DB_MONGO_CMD_DB_REPLY_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "CmdDB.h"

#include <string>
#include <vector>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

struct CmdReply
{
    double                      ok;
    std::size_t                 n;
    std::string                 errmsg;
    std::string                 codeName;
    int                         code;
    std::vector<WriteErrors>    writeErrors;
    WriteConcernError           writeConcernError;
};

class CmdDB_Reply: public Op_Reply<std::vector<CmdReply>, ViewBackInsertContainer<std::vector<CmdReply>>>
{
    public:
        std::vector<CmdReply>   reply;
        CmdDB_Reply()
            : Op_Reply(reply) // | FilterBackInserter{})
        {}
        std::size_t replyCount() const;
        virtual bool isOk() const override;
        virtual std::string getHRErrorMessage() const override;

    protected:
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Reply> const& reply);
        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Reply const&>(*this));}
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReply,           ok, n, writeErrors, writeConcernError, errmsg, codeName, code);

#endif
