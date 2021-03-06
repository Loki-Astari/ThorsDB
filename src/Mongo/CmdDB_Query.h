#ifndef THORSANVIL_DB_MONGO_CMD_DB_QUERY_H
#define THORSANVIL_DB_MONGO_CMD_DB_QUERY_H

// https://docs.mongodb.com/manual/reference/command/nav-crud/

/*
 * $    Usage:  CmdDB_Query
 *
 * >        This is not designed to be used directly. See CmdDB_Find.h for example.
 * >        It is expected for a CmdDB that you create an appropriate typedef.
 */
#include "BaseCmd.h"
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
        CmdDB_Query(std::string db, std::string collection, Op_QueryOptions const& option, Args&&... args);
        template<typename Doc = Document, HasOptions<Doc> = true, typename... Args>
        CmdDB_Query(std::string db, std::string collection, Op_QueryOptions const& option, typename Doc::Options const& docOpt, Args&&... args);

        // Insert & FindModify
        CmdDB_Query& byPass();

        // Insert & Delete
        CmdDB_Query& unordered();

        // Insert & Delete & FindModify
        CmdDB_Query& setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);

        // Insert & Find & FindModify & GetLastError & GetMore
        CmdDB_Query& setComment(std::string c);

        // Find & FindModify
        CmdDB_Query& addFileds(std::initializer_list<std::string> fieldNames);
        CmdDB_Query& addHint(std::string hint);

        // Find & GetMore
        CmdDB_Query& setBatchSize(std::size_t val);
        CmdDB_Query& setMaxTimeout(std::size_t val);

        // Find
        CmdDB_Query& setSkip(std::size_t val);
        CmdDB_Query& setLimit(std::size_t val);
        CmdDB_Query& oneBatch(bool val = true);
        CmdDB_Query& addReadConcern(ReadConcernLevel val);
        CmdDB_Query& addMax(std::string field, int val);
        CmdDB_Query& addMin(std::string field, int val);
        CmdDB_Query& justKeys(bool val = false);
        CmdDB_Query& showId(bool val = true);
        CmdDB_Query& tailableCursor(bool val = true);
        CmdDB_Query& tailedCursorAwait(bool val = true);
        CmdDB_Query& setNoCursorTimeout(bool val = true);
        CmdDB_Query& setAllowPartialResults(bool val = true);
        CmdDB_Query& useDisk(bool val = true);

        // FindModify
        CmdDB_Query& setNew(bool val = true);
        CmdDB_Query& setUpsert(bool val = true);

        // GetLastError
        CmdDB_Query& waitFoolDiskFlush(bool val = true);
        CmdDB_Query& waitForReplication(std::int32_t count);
        CmdDB_Query& setWaitTimeout(std::int32_t millisec);

        std::ostream& printHR(std::ostream& stream) const {return stream << make_hr(static_cast<Op_Query<Document> const&>(*this));}
};

template<typename Document>
std::ostream& operator<<(std::ostream& stream, CmdDB_Query<Document> const& data);

}

#include "CmdDB_Query.tpp"

#endif
