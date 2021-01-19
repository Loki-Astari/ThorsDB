#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
struct Insert: public CmdDB_Base
{
    Insert(std::string const& collection, Document const& doc)
        : insert(collection)
        , documents(1, doc)
    {}
    template<typename I>
    Insert(std::string const& collection, I begin, I end)
        : insert(collection)
        , documents(begin, end)
    {}
    std::string                 insert;
    std::vector<Document>       documents;
};


template<typename Document>
using CmdDB_Insert      = CmdDB_Query<Insert<Document>>;

template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string const& db, std::string const& collection, QueryOptions&& options, I begin, I end)
{
    using Document = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(db, collection, std::move(options), begin, end);
}

}

ThorsAnvil_Template_ExpandTrait(1, ThorsAnvil::DB::Mongo::CmdDB_Base, ThorsAnvil::DB::Mongo::Insert, insert, documents);

#endif
