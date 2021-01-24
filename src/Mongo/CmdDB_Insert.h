#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
struct Insert
{
    public:
        template<typename I>
        Insert(std::string const& collection, I begin, I end);
        Insert(std::string const& collection, Document const& doc);

        void unordered();
        void byPass();
        void setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
        void setComment(std::string&& c);
    private:
        friend class ThorsAnvil::Serialize::Traits<Insert>;
        friend class ThorsAnvil::Serialize::Filter<Insert>;
        std::map<std::string, bool> filter  = {{"ordered", false}, {"writeConcern", false}, {"bypassDocumentValidation", false}, {"comment", false}};
        std::string                 insert;
        std::vector<Document>       documents;
        bool                        ordered                  = true;
        WriteConcern                writeConcern;
        bool                        bypassDocumentValidation = false;
        std::string                 comment;
};


template<typename Document>
using CmdDB_Insert      = CmdDB_Query<Insert<Document>>;

template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string const& db, std::string const& collection, Op_QueryOptions const& options, I begin, I end)
{
    using Document = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(db, collection, options, begin, end);
}
template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string const& db, std::string const& collection, Op_QueryOptions&& options, I begin, I end)
{
    using Document = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(db, collection, std::move(options), begin, end);
}

}

ThorsAnvil_Template_MakeFilter(1, ThorsAnvil::DB::Mongo::Insert,        filter);
ThorsAnvil_Template_MakeTrait(1,  ThorsAnvil::DB::Mongo::Insert,        insert, documents, ordered, writeConcern, bypassDocumentValidation, comment);

#include "CmdDB_Insert.tpp"

#endif
