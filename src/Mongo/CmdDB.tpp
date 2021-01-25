#ifndef THORSANVIL_DB_MONGO_CMD_DB_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_H
#error  "This should only be included from CmdDB.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename Opt, ValidCmdQryOption<Op_QueryOptions>, typename... Args>
CmdDB_Query<Document>::CmdDB_Query(std::string const& db, std::string const& collection, Opt&& options, Args&&... args)
    : Op_Query<Document>(db + ".$cmd", std::forward<Opt>(options), collection, std::forward<Args>(args)...)
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
CmdDB_Query<Document>& CmdDB_Query<Document>::setComment(std::string&& val)
{
    this->getQuery().setComment(std::forward<std::string>(val));
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    this->getQuery().setWrieConcern(w, j, wtimeout);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addFileds(std::initializer_list<std::string> const& fieldNames)
{
    this->getQuery().addFileds(fieldNames);
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addHint(std::string&& hint)
{
    this->getQuery().addHint(hint);
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
CmdDB_Query<Document>& CmdDB_Query<Document>::singleBatch()
{
    this->getQuery().singleBatch();
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
    this->getQuery().addReadConcern(val);
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addMax(std::string const& field, int val)
{
    this->getQuery().addMax(field, val);
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::addMin(std::string const& field, int val)
{
    this->getQuery().addMin(field, val);
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::justKeys()
{
    this->getQuery().justKeys();
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::showId()
{
    this->getQuery().showId();
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::tailableCursor()
{
    this->getQuery().tailableCursor();
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::tailedCursorAwait()
{
    this->getQuery().tailedCursorAwait();
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setNoCursorTimeout()
{
    this->getQuery().setNoCursorTimeout();
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setAllowPartialResults()
{
    this->getQuery().setAllowPartialResults();
    return *this;
}
template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::useDisk()
{
    this->getQuery().useDisk();
    return *this;
}

}

#endif
