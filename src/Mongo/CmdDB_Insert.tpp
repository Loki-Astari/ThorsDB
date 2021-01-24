#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_TPP

#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#error  "This should only be included from CmdDB_Insert.h"
#endif

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
Insert<Document>::Insert(std::string const& collection, Document const& doc)
    : insert(collection)
    , documents(1, doc)
{}

template<typename Document>
template<typename I>
Insert<Document>::Insert(std::string const& collection, I begin, I end)
    : insert(collection)
    , documents(begin, end)
{}

template<typename Document>
void Insert<Document>::unordered()
{
    ordered = false;
    filter["ordered"] = true;
}

template<typename Document>
void Insert<Document>::byPass()
{
    bypassDocumentValidation = false;
    filter["bypassDocumentValidation"] = true;
}

template<typename Document>
void Insert<Document>::setWrieConcern(int w, bool j, std::time_t wtimeout)
{
    writeConcern    = WriteConcern{w, j, wtimeout};
    filter["writeConcern"]  = true;
}

template<typename Document>
void Insert<Document>::setComment(std::string&& c)
{
    comment = c;
    filter["comment"] = true;
}

}

#endif
