#ifndef THORSANVIL_DB_MONGO_LIST_DATABASES_H
#define THORSANVIL_DB_MONGO_LIST_DATABASES_H

// https://github.com/mongodb/specifications/blob/master/source/enumerate-databases.rst

#include "Op_Query.h"
#include "Op_Reply.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/MongoUtility.h"
#include "ThorSerialize/JsonPrinter.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

struct ListDataBases
{
    bool    listDatabases = 1;
};

struct DBInfo
{
    std::string     name;
    double          sizeOnDisk;
    bool            empty;
};

struct ListDataBaseReply
{
    ListDataBaseReply()
        : ok(1.0)
    {}
    double              ok;
    // If ok is zero
    int                 code;
#pragma vera-pushoff
    std::string         $err;
#pragma vera-pop
    // Normal response
    std::vector<DBInfo> databases;
    double              totalSize;
};

class Op_QueryListDataBases: public Op_Query<ListDataBases>
{
    public:
        Op_QueryListDataBases()
            : Op_Query("admin.$cmd")
        {}
        friend std::ostream& operator<<(std::ostream& stream, Op_QueryListDataBases& data) {return data.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_QueryListDataBases> const& data);
};
using Op_ReplListDataBases = Op_Reply<ListDataBaseReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ListDataBases,      listDatabases);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DBInfo,             name, sizeOnDisk, empty);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ListDataBaseReply,  ok, code, $err, databases, totalSize);

#endif
