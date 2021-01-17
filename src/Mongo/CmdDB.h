#ifndef THORSANVIL_DB_MONGO_CMD_DB_H
#define THORSANVIL_DB_MONGO_CMD_DB_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "Op_Query.h"
#include "Op_Reply.h"

namespace ThorsAnvil::DB::Mongo
{

struct WriteConcern
{
    int             w;
    bool            j;
    std::time_t     wtimeout;
};

struct WriteErrors
{
    std::size_t                 index;
    int                         code;
    std::string                 errmsg;
};

struct WriteConcernError
{
    int                         code;
    std::string                 errmsg;
};

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

template<typename Document>
class CmdDB_Query: public Op_Query<Document>
{
    public:
        template<typename... Args>
        CmdDB_Query(std::string const& db, std::string const& collection, QueryOptions&& options, Args&&... args)
            : Op_Query<Document>(db + ".$cmd", std::forward<QueryOptions>(options), 1, 0, collection, std::forward<Args>(args)...)
        {}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Query> const& data);
        friend std::ostream& operator<<(std::ostream& stream, CmdDB_Query const& data) {return data.print(stream);}
};

using CmdDB_Reply = Op_Reply<CmdReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcern,       w, j, wtimeout);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteErrors,        index, code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcernError,  code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReply,           ok, n, writeErrors, writeConcernError, errmsg, codeName, code);

#endif
