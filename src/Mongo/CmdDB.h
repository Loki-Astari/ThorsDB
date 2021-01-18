#ifndef THORSANVIL_DB_MONGO_CMD_DB_H
#define THORSANVIL_DB_MONGO_CMD_DB_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "Op_Query.h"
#include "Op_Reply.h"

namespace ThorsAnvil::DB::Mongo
{

struct WriteConcern
{
    WriteConcern(int w = 1, bool j = false, std::time_t wtimeout = 0)
        : w(w)
        , j(j)
        , wtimeout(wtimeout)
    {}
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
        CmdDB_Query& unordered()                        {this->getQuery().unordered();    return *this;}
        CmdDB_Query& byPass()                           {this->getQuery().byPass();       return *this;}
        CmdDB_Query& setComment(std::string const& c)   {this->getQuery().setComment(c);  return *this;}
        CmdDB_Query& setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0)
        {
            this->getQuery().setWrieConcern(w, j, wtimeout);
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Query> const& data);
        friend std::ostream& operator<<(std::ostream& stream, CmdDB_Query const& data) {return data.print(stream);}
};

class CmdDB_Base
{
    public:
        CmdDB_Base(bool ordered, WriteConcern concern, bool byPass, std::string const& comment = "")
            : filter{{"ordered", false}, {"writeConcern", false}, {"bypassDocumentValidation", false}, {"comment", false}}
            , ordered(ordered)
            , writeConcern(concern)
            , bypassDocumentValidation(byPass)
            , comment(comment)
        {
            std::cerr << "CmdDB_Base:\n";
        }
        CmdDB_Base()
        {}
        void unordered()
        {
            ordered = false;
            filter["ordered"] = true;
        }
        void byPass()
        {
            bypassDocumentValidation = true;
            filter["bypassDocumentValidation"] = true;
        }
        void setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0)
        {
            writeConcern    = WriteConcern{w, j, wtimeout};
            filter["writeConcern"]  = true;
        }
        void setComment(std::string const& c)
        {
            comment = c;
            filter["comment"] = true;
        }
    private:
        friend class ThorsAnvil::Serialize::Traits<CmdDB_Base>;
        friend class ThorsAnvil::Serialize::TraitsMemberFilter<CmdDB_Base>;
        std::map<std::string, bool> filter;
        bool                        ordered                  = true;
        WriteConcern                writeConcern;
        bool                        bypassDocumentValidation = false;
        std::string                 comment;
};

using CmdDB_Reply = Op_Reply<CmdReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcern,       w, j, wtimeout);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteErrors,        index, code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcernError,  code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReply,           ok, n, writeErrors, writeConcernError, errmsg, codeName, code);

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdDB_Base,         ordered, writeConcern, bypassDocumentValidation, comment);
ThorsAnvil_MakeTraitName_Filter(ThorsAnvil::DB::Mongo::CmdDB_Base, filter);

#endif
