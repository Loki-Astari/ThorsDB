#ifndef THORSANVIL_DB_MONGO_CMD_DB_H
#define THORSANVIL_DB_MONGO_CMD_DB_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "Op_Query.h"
#include "Op_Reply.h"

namespace ThorsAnvil::DB::Mongo
{

struct Collation
{
    std::string     locale;
    bool            caseLevel;
    std::string     caseFirst;
    int             strength;
    bool            numericOrdering;
    std::string     alternate;
    std::string     maxVariable;
    bool            backwards;
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

class CmdDB_Reply: public Op_Reply<CmdReply>
{
    public:
        bool replyCount() const;
        virtual bool isOk() const override;
        virtual std::string getHRErrorMessage() const override;
    protected:
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Reply> const& reply);
        std::ostream& printHR(std::ostream& stream) const {return Op_Reply::printHR(stream);}
};


struct WriteConcern
{
    WriteConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);

    int             w;
    bool            j;
    std::time_t     wtimeout;
};

class CmdDB_Base
{
    public:
        CmdDB_Base();

        void unordered();
        void byPass();
        void setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
        void setComment(std::string const& c);

    private:
        friend class ThorsAnvil::Serialize::Traits<CmdDB_Base>;
        friend class ThorsAnvil::Serialize::Filter<CmdDB_Base>;
        std::map<std::string, bool> filter;
        bool                        ordered                  = true;
        WriteConcern                writeConcern;
        bool                        bypassDocumentValidation = false;
        std::string                 comment;
};

template<typename Document>
class CmdDB_Query: public Op_Query<Document>
{
    public:
        template<typename... Args>
        CmdDB_Query(std::string const& db, std::string const& collection, QueryOptions&& options, Args&&... args);

        CmdDB_Query& unordered();
        CmdDB_Query& byPass();
        CmdDB_Query& setComment(std::string const& c);
        CmdDB_Query& setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Query> const& data);
        friend std::ostream& operator<<(std::ostream& stream, CmdDB_Query const& data) {return data.print(stream);}
};


}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Collation,          locale, caseLevel, strength, numericOrdering, alternate, maxVariable, backwards);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcern,       w, j, wtimeout);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteErrors,        index, code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcernError,  code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReply,           ok, n, writeErrors, writeConcernError, errmsg, codeName, code);

ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::CmdDB_Base,        filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdDB_Base,         ordered, writeConcern, bypassDocumentValidation, comment);

#include "CmdDB.tpp"

#endif
