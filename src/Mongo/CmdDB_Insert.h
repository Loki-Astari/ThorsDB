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
        Insert(std::string const& collection, Document const& doc)
            : insert(collection)
            , documents(1, doc)
        {}
        template<typename I>
        Insert(std::string const& collection, I begin, I end)
            : insert(collection)
            , documents(begin, end)
        {}

        void unordered()
        {
            ordered = false;
            filter["ordered"] = true;
        }
        void byPass()
        {
            bypassDocumentValidation = false;
            filter["bypassDocumentValidation"] = true;
        }
        void setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0)
        {
            writeConcern    = WriteConcern{w, j, wtimeout};
            filter["writeConcern"]  = true;
        }
        void setComment(std::string&& c)
        {
            comment = c;
            filter["comment"] = true;
        }
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
make_CmdDB_Insert(std::string const& db, std::string const& collection, QueryOptions&& options, I begin, I end)
{
    using Document = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(db, collection, std::move(options), begin, end);
}

}

ThorsAnvil_Template_MakeFilter(1, ThorsAnvil::DB::Mongo::Insert,        filter);
ThorsAnvil_Template_MakeTrait(1,  ThorsAnvil::DB::Mongo::Insert,        insert, documents, ordered, writeConcern, bypassDocumentValidation, comment);

#endif
