#ifndef THORSANVIL_DB_MONGO_CMD_DB_INSERT_H
#define THORSANVIL_DB_MONGO_CMD_DB_INSERT_H

// https://docs.mongodb.com/manual/reference/command/insert/#dbcmd.insert

/* $    Usage: CmdDB_Insert
 * $        Document:           Serializeable object that is sent/retrieved to/from Mongo.
 * $        Document-View:      Either a single "Document" item or a range of "Document" range: [begin(),  end())
 * $        connection:         connection to mongo DB or a stream.
 * $        Op_QueryOptions:    See: Op_Query.h
 * $        DeleteOptions:      See: below
 *
 * >    connection << send_CmdDB_Insert("db", "collection" [, Op_Query_Options] [, InsertOptions] [,<Document-View>]);
 * >    connection >> get_CmdDB_InsertReply();
 */

#include "Util.h"
#include "View.h"
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

template<typename View>
struct Insert: public InsertOptional
{
    public:
        using Options = InsertOptions;

        Insert(InsertOptions const& options, std::string collection, View&& view);

    private:
        friend class ThorsAnvil::Serialize::Traits<Insert>;
        std::string                 insert;
        View                        documents;
};

template<typename Document>
using CmdDB_Insert      = CmdDB_Query<Insert<Document>>;

using CmdDB_InsertReply = CmdDB_Reply<CmdReply>;

template<typename Range>
CmdDB_Insert<ViewType<Range>> send_CmdDB_Insert(std::string db, std::string collection, Range&& r);

template<typename Range>
CmdDB_Insert<ViewType<Range>> send_CmdDB_Insert(std::string db, std::string collection, InsertOptions const& options, Range&& r);

template<typename Range>
CmdDB_Insert<ViewType<Range>> send_CmdDB_Insert(std::string db, std::string collection, Op_QueryOptions const& options, Range&& r);

template<typename Range>
CmdDB_Insert<ViewType<Range>> send_CmdDB_Insert(std::string db, std::string collection, Op_QueryOptions const& options, InsertOptions const& insertOpt, Range&& r);

inline CmdDB_InsertReply get_CmdDB_InsertReply() {return CmdDB_InsertReply{};}
}

ThorsAnvil_MakeFilter(ThorsAnvil::DB::Mongo::InsertOptional,                filter);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::InsertOptional,                 ordered, writeConcern, bypassDocumentValidation, comment);
ThorsAnvil_Template_ExpandTrait(1,  ThorsAnvil::DB::Mongo::InsertOptional,  ThorsAnvil::DB::Mongo::Insert,        insert, documents);

#include "CmdDB_Insert.tpp"

#endif
