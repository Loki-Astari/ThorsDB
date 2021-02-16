#ifndef THORSANVIL_DB_MONGO_BASE_CMD_H
#define THORSANVIL_DB_MONGO_BASE_CMD_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "Op_Query.h"
#include "Op_Reply.h"
#include "ThorSerialize/Traits.h"

#include <string>

namespace ThorsAnvil::DB::Mongo
{

enum class ReadConcernLevel {local, available, majority, linearizable};

struct Collation
{
    friend bool operator!=(Collation const& lhs, Collation const& rhs);

    std::string     locale;
    bool            caseLevel           = false;
    std::string     caseFirst;
    int             strength            = 0;
    bool            numericOrdering     = false;
    std::string     alternate;
    std::string     maxVariable;
    bool            backwards           = false;
};

struct WriteErrors
{
    std::size_t                 index;
    int                         code;
    std::string                 errmsg;
};

struct ReadConcern
{
    friend bool operator!=(ReadConcern const& lhs, ReadConcern const& rhs);
    ReadConcernLevel    level = ReadConcernLevel::local;
};

struct WriteConcernError
{
    int                         code;
    std::string                 errmsg;
};

struct WriteConcern
{
    WriteConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
    friend bool operator!=(WriteConcern const& lhs, WriteConcern const& rhs);

    int             w           = 0;
    bool            j           = 0;
    std::time_t     wtimeout    = 0;
};

struct CmdReplyBase
{
    using UserData  = void;

    double                      ok              = 0.0;
    std::string                 errmsg;
    std::string                 codeName;
    int                         code            = 0;

    bool isOk() const;
    std::string getHRErrorMessage() const;
};

struct CmdReply: public CmdReplyBase
{
    using UserData  = std::size_t;
    using Reference = std::reference_wrapper<UserData>;

    CmdReply(UserData& count)
        : n(count)
    {}

    Reference                   n;
    std::vector<WriteErrors>    writeErrors;
    WriteConcernError           writeConcernError;

    std::string getHRErrorMessage() const;
};

}

ThorsAnvil_MakeEnum(ThorsAnvil::DB::Mongo::ReadConcernLevel,    local, available, majority, linearizable);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Collation,          locale, caseLevel, strength, numericOrdering, alternate, maxVariable, backwards);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcern,       w, j, wtimeout);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteErrors,        index, code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcernError,  code, errmsg);

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,       ok, errmsg, codeName, code);
ThorsAnvil_ExpandTrait(ThorsAnvil::DB::Mongo::CmdReplyBase,
                       ThorsAnvil::DB::Mongo::CmdReply,         n, writeErrors, writeConcernError);

#include "BaseCmd.tpp"

#endif
