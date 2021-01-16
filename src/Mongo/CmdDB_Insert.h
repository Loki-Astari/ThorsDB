#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

#include "Op_Query.h"
#include "Op_Reply.h"

namespace ThorsAnvil::DB::Mongo
{

struct WriteConcern
{
    int         w;
    bool        j;
    std::time_t wtimeout;
};

template<typename Document>
struct Insert
{
    template<typename I>
    Insert(std::string const& collection, I begin, I end, bool ordered, WriteConcern concern, bool byPass, std::string const& comment = "")
        : insert(collection)
        , documents(begin, end)
        , ordered(ordered)
        //, writeConcern(concern)
        , bypassDocumentValidation(byPass)
        , comment(comment)
    {
        std::cerr << "Insert: " << documents.size() << "\n";
    }
    std::string                 insert;
    std::vector<Document>       documents;
    bool                        ordered;
    //WriteConcern                writeConcern;
    bool                        bypassDocumentValidation;
    std::string                 comment;
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

struct InsertReply
{
    double                      ok;
    std::size_t                 n;
    std::vector<WriteErrors>    writeErrors;
    WriteConcernError           writeConcernError;
};

template<typename Document>
class CmdAdmInsert: public Op_Query<Insert<Document>>
{
    public:
        template<typename I>
        CmdAdmInsert(std::string const& collection, I begin, I end, bool ordered, WriteConcern concern, bool byPass, std::string const& comment)
            : Op_Query<Insert<Document>>("test.$cmd", QueryOptions{}, 1, 0, collection, begin, end, ordered, concern, byPass, comment)
        {}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdAdmInsert> const& data);
        friend std::ostream& operator<<(std::ostream& stream, CmdAdmInsert const& data) {return data.print(stream);}
};
using CmdAdmInsertReply = Op_Reply<InsertReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcern,       w, j, wtimeout);
ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::Insert, insert, documents, ordered, bypassDocumentValidation, comment);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteErrors,        index, code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcernError,  code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::InsertReply,        ok, n, writeErrors, writeConcernError);

#endif
