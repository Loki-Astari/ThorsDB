#ifndef THORSANVIL_DB_MONGO_CMD_DB_H
#define THORSANVIL_DB_MONGO_CMD_DB_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "Op_Query.h"
#include "Op_Reply.h"
#include "ThorSerialize/SerUtil.h"

namespace ThorsAnvil::DB::Mongo
{

enum class ReadConcernLevel {local, available, majority, linearizable};

struct Collation
{
    friend bool operator!=(Collation const& lhs, Collation const& rhs)
    {
        return std::tie(lhs.locale, lhs.caseLevel, lhs.caseFirst, lhs.strength, lhs.numericOrdering, lhs.alternate, lhs.maxVariable, lhs.backwards)
            != std::tie(rhs.locale, rhs.caseLevel, rhs.caseFirst, rhs.strength, rhs.numericOrdering, rhs.alternate, rhs.maxVariable, rhs.backwards);
    }
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

struct WriteConcernError
{
    int                         code;
    std::string                 errmsg;
};

struct WriteConcern
{
    WriteConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
    friend bool operator!=(WriteConcern const& lhs, WriteConcern const& rhs)
    {
        return std::tie(lhs.w, lhs.j, lhs.wtimeout) != std::tie(rhs.w, rhs.j, rhs.wtimeout);
    }

    int             w           = 0;
    bool            j           = 0;
    std::time_t     wtimeout    = 0;
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
        std::size_t replyCount() const;
        virtual bool isOk() const override;
        virtual std::string getHRErrorMessage() const override;
    protected:
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Reply> const& reply);
        std::ostream& printHR(std::ostream& stream) const {return Op_Reply::printHR(stream);}
};

template<typename Actual>
using ValidCmdQryOption = DerivedOption<Actual, Op_QueryOptions>;

template<typename Document>
class CmdDB_Query: public Op_Query<Document>
{
    public:
        template<typename Opt = Op_QueryOptions, ValidCmdQryOption<Op_QueryOptions> = true, typename... Args>
        CmdDB_Query(std::string const& db, std::string const& collection, Opt&& options, Args&&... args);

        // Insert
        CmdDB_Query& byPass();

        // Insert & Delete
        CmdDB_Query& unordered();
        CmdDB_Query& setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);

        // Insert & Find
        CmdDB_Query& setComment(std::string&& c);

        // Find
        CmdDB_Query& addFileds(std::initializer_list<std::string> const& fieldNames);
        CmdDB_Query& addHint(std::string&& hint);
        CmdDB_Query& setSkip(std::size_t val);
        CmdDB_Query& setLimit(std::size_t val);
        CmdDB_Query& setBatchSize(std::size_t val);
        CmdDB_Query& oneBatch(bool val = true);
        CmdDB_Query& setMaxTimeout(std::size_t val);
        CmdDB_Query& addReadConcern(ReadConcernLevel val);
        CmdDB_Query& addMax(std::string const& field, int val);
        CmdDB_Query& addMin(std::string const& field, int val);
        CmdDB_Query& justKeys(bool val = false);
        CmdDB_Query& showId(bool val = true);
        CmdDB_Query& tailableCursor(bool val = true);
        CmdDB_Query& tailedCursorAwait(bool val = true);
        CmdDB_Query& setNoCursorTimeout(bool val = true);
        CmdDB_Query& setAllowPartialResults(bool val = true);
        CmdDB_Query& useDisk(bool val = true);

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<CmdDB_Query> const& data);
        friend std::ostream& operator<<(std::ostream& stream, CmdDB_Query const& data) {return data.print(stream);}
};

}

ThorsAnvil_MakeEnum(ThorsAnvil::DB::Mongo::ReadConcernLevel,    local, available, majority, linearizable);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::Collation,          locale, caseLevel, strength, numericOrdering, alternate, maxVariable, backwards);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcern,       w, j, wtimeout);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteErrors,        index, code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::WriteConcernError,  code, errmsg);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::CmdReply,           ok, n, writeErrors, writeConcernError, errmsg, codeName, code);

#include "CmdDB.tpp"

#endif
