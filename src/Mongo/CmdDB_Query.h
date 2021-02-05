#ifndef THORSANVIL_DB_MONGO_CMD_DB_QUERY_H
#define THORSANVIL_DB_MONGO_CMD_DB_QUERY_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

#include "CmdDB.h"
#include "Op_Query.h"

#include <string>
#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
class CmdDB_Query: public Op_Query<Document>
{
    public:
        template<typename Doc = Document, NoOptions<Doc> = true, typename... Args>
        CmdDB_Query(std::string db, std::string collection, Op_QueryOptions option, Args&&... args);
        template<typename Doc = Document, HasOptions<Doc> = true, typename... Args>
        CmdDB_Query(std::string db, std::string collection, Op_QueryOptions option, typename Doc::Options docOpt, Args&&... args);

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
        friend std::ostream& operator<<(std::ostream& stream, CmdDB_Query const& data) {return stream << static_cast<Op_Query<Document> const&>(data);}
        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Query<Document> const&>(*this));}
};

}

#include "CmdDB_Query.tpp"

#endif
