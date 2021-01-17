#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

#include "CmdDB.h"

namespace ThorsAnvil::DB::Mongo
{

template<typename Document>
struct Insert
{
    template<typename I>
    Insert(std::string const& collection, I begin, I end, bool ordered, WriteConcern concern, bool byPass, std::string const& comment = "")
        : insert(collection)
        , documents(begin, end)
        , ordered(ordered)
        //, writeConcern(concern)
        , bypassDocumentValidation(byPass)
        , comment(comment)
    {
        std::cerr << "Insert: " << documents.size() << "\n";
    }
    std::string                 insert;
    std::vector<Document>       documents;
    bool                        ordered;
    //WriteConcern                writeConcern;
    bool                        bypassDocumentValidation;
    std::string                 comment;
};


template<typename Document>
using CmdDB_Insert      = CmdDB_Query<Insert<Document>>;

}

ThorsAnvil_Template_MakeTrait(1, ThorsAnvil::DB::Mongo::Insert, insert, documents, ordered, bypassDocumentValidation, comment);

#endif
