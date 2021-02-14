#ifndef THORSANVIL_DB_MONGO_CMD_DB_REPLY_H
#define THORSANVIL_DB_MONGO_CMD_DB_REPLY_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

/*
 * $    Usage:  CmdDB_Reply
 *
 * >        This is not designed to be used directly. See CmdDB_Find.h for example.
 * >        It is expected for a CmdDB that you create an appropriate typedef.
 */

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
class CmdDB_Reply: public Op_Reply<ViewType<Document>>
{
    public:
        Document   reply;
        template<typename Doc = Document, NoOptions<Doc> = true>
        CmdDB_Reply()
            : Op_Reply<ViewType<Document>>(reply)
        {}
        template<typename Doc = Document, HasOptions<Doc> = true>
        CmdDB_Reply(std::vector<typename Doc::Options>& container)
            : Op_Reply<ViewType<Document>>(reply)
            , reply(container)
        {}
        virtual bool isOk() const override;
        virtual std::string getHRErrorMessage() const override;

        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Reply<ViewType<Document>> const&>(*this));}
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReply,           ok, n, writeErrors, writeConcernError, errmsg, codeName, code);

#include "CmdDB_Reply.tpp"

#endif
