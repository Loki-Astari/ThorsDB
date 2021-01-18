#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
struct Insert: public CmdDB_Base
{
    Insert(std::string const& collection, Document const& doc)//, bool ordered, WriteConcern concern, bool byPass, std::string const& comment = "")
        //: CmdDB_Base(ordered, concern, byPass, comment)
        : insert(collection)
        , documents(1, doc)
    {
        std::cerr << "Insert: " << documents.size() << "\n";
    }
    template<typename I>
    Insert(std::string const& collection, I begin, I end)//, bool ordered, WriteConcern concern, bool byPass, std::string const& comment = "")
        //: CmdDB_Base(ordered, concern, byPass, comment)
        : insert(collection)
        , documents(begin, end)
    {
        std::cerr << "Insert: " << documents.size() << "\n";
    }
    std::string                 insert;
    std::vector<Document>       documents;
};


template<typename Document>
using CmdDB_Insert      = CmdDB_Query<Insert<Document>>;

template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string const& db, std::string const& collection, QueryOptions&& options, I begin, I end)//, bool ordered, WriteConcern concern, bool byPass, std::string const& comment = "")
{
    using Document = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(db, collection, std::move(options), begin, end);//, ordered, concern, byPass, comment);
}

}

ThorsAnvil_Template_ExpandTrait(1, ThorsAnvil::DB::Mongo::CmdDB_Base, ThorsAnvil::DB::Mongo::Insert, insert, documents);

#endif
