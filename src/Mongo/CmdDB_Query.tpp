#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_QUERY_TPP
#define THORS_ANVIL_DB_MONGO_CMD_DB_QUERY_TPP

#ifndef THORS_ANVIL_DB_MONGO_CMD_DB_QUERY_H
#error  "This should only be included from CmdDB_Query.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename Doc, NoOptions<Doc>, typename... Args>
CmdDB_Query<Document>::CmdDB_Query(std::string db, std::string collection, Op_QueryOptions const& options, Args&&... args)
    : Op_Query<Document>(db + ".$cmd", options, collection, std::forward<Args>(args)...)
{}

template<typename Document>
template<typename Doc, HasOptions<Doc>, typename... Args>
CmdDB_Query<Document>::CmdDB_Query(std::string db, std::string collection, Op_QueryOptions const& options, typename Doc::Options const& docOpt, Args&&... args)
    : Op_Query<Document>(db + ".$cmd", options, docOpt, std::move(collection), std::forward<Args>(args)...)
{}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::unordered()
{
    this->getQuery().unordered();
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::byPass()
{
    this->getQuery().byPass();
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setComment(std::string val)
{
    this->getQuery().setComment(std::move(val));
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    this->getQuery().setWrieConcern(w, j, wtimeout);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addFileds(std::initializer_list<std::string> fieldNames)
{
    this->getQuery().addFileds(std::move(fieldNames));
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addHint(std::string hint)
{
    this->getQuery().addHint(std::move(hint));
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setSkip(std::size_t val)
{
    this->getQuery().setSkip(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setLimit(std::size_t val)
{
    this->getQuery().setLimit(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setBatchSize(std::size_t val)
{
    this->getQuery().setBatchSize(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::oneBatch(bool val)
{
    this->getQuery().oneBatch(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setMaxTimeout(std::size_t val)
{
    this->getQuery().setMaxTimeout(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addReadConcern(ReadConcernLevel val)
{
    this->getQuery().addReadConcern(std::move(val));
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addMax(std::string field, int val)
{
    this->getQuery().addMax(std::move(field), val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addMin(std::string field, int val)
{
    this->getQuery().addMin(std::move(field), val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::justKeys(bool val)
{
    this->getQuery().justKeys(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::showId(bool val)
{
    this->getQuery().showId(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::tailableCursor(bool val)
{
    this->getQuery().tailableCursor(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::tailedCursorAwait(bool val)
{
    this->getQuery().tailedCursorAwait(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setNoCursorTimeout(bool val)
{
    this->getQuery().setNoCursorTimeout(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setAllowPartialResults(bool val)
{
    this->getQuery().setAllowPartialResults(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::useDisk(bool val)
{
    this->getQuery().useDisk(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setNew(bool val)
{
    this->getQuery().setNew(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setUpsert(bool val)
{
    this->getQuery().setUpsert(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::waitFoolDiskFlush(bool val)
{
    this->getQuery().waitFoolDiskFlush(val);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::waitForReplication(std::int32_t count)
{
    this->getQuery().waitForReplication(count);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setWaitTimeout(std::int32_t millisec)
{
    this->getQuery().setWaitTimeout(millisec);
    return *this;
}

template<typename Document>
std::ostream& operator<<(std::ostream& stream, CmdDB_Query<Document> const& data)
{
    return stream << static_cast<Op_Query<Document> const&>(data);
}

}

#endif
