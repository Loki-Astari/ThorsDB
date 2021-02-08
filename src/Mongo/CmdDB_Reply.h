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
    double                      ok              = 0;
    std::size_t                 n               = 0;
    std::string                 errmsg;
    std::string                 codeName;
    int                         code            = 0;
    std::vector<WriteErrors>    writeErrors;
    WriteConcernError           writeConcernError;

    bool isOk() const;
    std::string getHRErrorMessage() const;
};

// class CmdDB_Reply: public Op_Reply<std::vector<CmdReply>, ViewBackInsertContainer<std::vector<CmdReply>>>
template<typename Document>
class CmdDB_Reply: public Op_Reply<Document>
{
    public:
        Document   reply;
        CmdDB_Reply()
            : Op_Reply<Document>(reply)
        {}
        virtual bool isOk() const override;
        virtual std::string getHRErrorMessage() const override;

    protected:
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Reply> const& reply);
        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Reply<Document> const&>(*this));}
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReply,           ok, n, writeErrors, writeConcernError, errmsg, codeName, code);

#include "CmdDB_Reply.tpp"

#endif
