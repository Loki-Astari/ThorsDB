#ifndef THORSANVIL_DB_MONGO_CMD_DB_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_TPP

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
template<typename... Args>
CmdDB_Query<Document>::CmdDB_Query(std::string const& db, std::string const& collection, QueryOptions&& options, Args&&... args)
    : Op_Query<Document>(db + ".$cmd", std::forward<QueryOptions>(options), 1, 0, collection, std::forward<Args>(args)...)
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
CmdDB_Query<Document>& CmdDB_Query<Document>::setComment(std::string const& c)
{
    this->getQuery().setComment(c);
    return *this;
}

template<typename Document>
CmdDB_Query<Document>& CmdDB_Query<Document>::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    this->getQuery().setWrieConcern(w, j, wtimeout);
    return *this;
}

}

#endif
