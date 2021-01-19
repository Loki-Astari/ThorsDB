#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_DELETE_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_DELETE_H
#error  "This should only be included from CmdDB_Delete.h"
#endif


namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
DeleteQuery<Document>::DeleteQuery(Document const& q)
    : q(q)
    , limit(0)
    , filter{{"collation", false}, {"hint", false}}
{}

template<typename Document>
Delete<Document>::Delete(std::string const& collection, DeleteQuery<Document> const& doc)
    : deleteFrom(collection)
    , deletes(1, doc)
{}

template<typename Document>
template<typename I>
Delete<Document>::Delete(std::string const& collection, I begin, I end)
    : deleteFrom(collection)
    , deletes(begin, end)
{}

template<typename Document>
void Delete<Document>::unordered()
{
    ordered = false;
    filter["ordered"] = true;
}

template<typename Document>
void Delete<Document>::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    writeConcern    = WriteConcern{w, j, wtimeout};
    filter["writeConcern"]  = true;
}

}

#endif
