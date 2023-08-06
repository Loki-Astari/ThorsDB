#ifndef THORS_ANVIL_DB_MONGO_CMD_ADM_REPLY_H
#define THORS_ANVIL_DB_MONGO_CMD_ADM_REPLY_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

/*
 * $    Usage:  CmdAdm_Reply
 *
 * >        This is not designed to be used directly. See AdmCmd_ListDataBases.h for example.
 * >        It is expected for a CmdAdm that you create an appropriate typedef.
 */

#include "BaseCmd.h"

#include <string>
#include <vector>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

// class CmdAdm_Reply: public Op_Reply<std::vector<CmdReply>, ViewBackInsertContainer<std::vector<CmdReply>>>
template<typename Document>
class CmdAdm_Reply: public Op_Reply<ViewType<Document>>
{
    public:
        Document   reply;
        template<typename Doc = Document, NoUserData<Doc> = true>
        CmdAdm_Reply()
            : Op_Reply<ViewType<Document>>(reply)
        {}
        template<typename Doc = Document, HasUserData<Doc> = true>
        CmdAdm_Reply(typename Doc::UserData& value)
            : Op_Reply<ViewType<Document>>(reply)
            , reply(value)
        {}
        virtual bool isOk() const override;
        virtual std::string getHRErrorMessage() const override;

        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Reply<ViewType<Document>> const&>(*this));}
};

}

#include "CmdAdm_Reply.tpp"

#endif
