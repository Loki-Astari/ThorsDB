#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

#include "CmdDB.h"
#include "CmdDB_Query.h"
#include "CmdDB_Reply.h"

#include <map>
#include <string>
#include <vector>

namespace ThorsAnvil::DB::Mongo
{

struct InsertOptions
{
    bool                        ordered                  = true;
    WriteConcern                writeConcern;
    bool                        bypassDocumentValidation = false;
    std::string                 comment;
};

class InsertOptional: public InsertOptions
{
    public:
        InsertOptional(InsertOptions const& options);

        void unordered(bool v = true);
        void byPass(bool v = true);
        void setWrieConcern(int w = 1, bool j = false, std::time_t wtimeout = 0);
        void setComment(std::string c = "");
    protected:
        void    updateFilter();
        friend class ThorsAnvil::Serialize::Traits<InsertOptional>;
        friend class ThorsAnvil::Serialize::Filter<InsertOptional>;
        std::map<std::string, bool> filter  = {{"ordered", false}, {"writeConcern", false}, {"bypassDocumentValidation", false}, {"comment", false}};
        bool                        ordered;
        WriteConcern                writeConcern;
        bool                        bypassDocumentValidation;
        std::string                 comment;
};

template<typename Document>
struct Insert: public InsertOptional
{
    public:
        using Options = InsertOptions;

        template<typename I>
        Insert(InsertOptions const& options, std::string collection, I begin, I end);

    private:
        friend class ThorsAnvil::Serialize::Traits<Insert>;
        std::string                 insert;
        std::vector<Document>       documents;
};

template<typename Document>
using CmdDB_Insert      = CmdDB_Query<Insert<Document>>;

using CmdDB_InsertReply = CmdDB_Reply<CmdReply>;

template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string db, std::string collection, I begin, I end)
{
    using Document  = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(std::move(db), std::move(collection), {}, {}, begin, end);
}

template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string db, std::string collection, InsertOptions const& options, I begin, I end)
{
    using Document  = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(std::move(db), std::move(collection), {}, options, begin, end);
}

template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string db, std::string collection, Op_QueryOptions const& options, I begin, I end)
{
    using Document  = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(std::move(db), std::move(collection), options, {}, begin, end);
}

template<typename I>
CmdDB_Insert<typename std::iterator_traits<I>::value_type>
make_CmdDB_Insert(std::string db, std::string collection, Op_QueryOptions const& options, InsertOptions const& insertOpt, I begin, I end)
{
    using Document  = typename std::iterator_traits<I>::value_type;
    return CmdDB_Insert<Document>(std::move(db), std::move(collection), options, insertOpt, begin, end);
}

}

ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::InsertOptional,                filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::InsertOptional,                 ordered, writeConcern, bypassDocumentValidation, comment);
ThorsAnvil_Template_ExpandTrait(1,  ThorsAnvil::DB::Mongo::InsertOptional,  ThorsAnvil::DB::Mongo::Insert,        insert, documents);

#include "CmdDB_Insert.tpp"

#endif
