#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_REPLY_H
#define THORS_ANVIL_DB_MONGO_CMD_DB_REPLY_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

/*
 * $    Usage:  CmdDB_Reply
 *
 * >        This is not designed to be used directly. See CmdDB_Find.h for example.
 * >        It is expected for a CmdDB that you create an appropriate typedef.
 */

#include "BaseCmd.h"

#include <string>
#include <vector>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

// class CmdDB_Reply: public Op_Reply<std::vector<CmdReply>, ViewBackInsertContainer<std::vector<CmdReply>>>
template<typename Document>
class CmdDB_Reply: public Op_Reply<ViewType<Document>>
{
    public:
        Document   reply;
        template<typename Doc = Document, NoUserData<Doc> = true>
        CmdDB_Reply()
            : Op_Reply<ViewType<Document>>(reply)
        {}
        template<typename Doc = Document, HasUserData<Doc> = true>
        CmdDB_Reply(typename Doc::UserData& value)
            : Op_Reply<ViewType<Document>>(reply)
            , reply(value)
        {}
        virtual bool isOk() const override;
        virtual std::string getHRErrorMessage() const override;

        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Reply<ViewType<Document>> const&>(*this));}
};

}

#include "CmdDB_Reply.tpp"

#endif
