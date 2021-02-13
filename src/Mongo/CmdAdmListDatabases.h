#ifndef THORSANVIL_DB_MONGO_CMD_ADM_LIST_DATABASES_H
#define THORSANVIL_DB_MONGO_CMD_ADM_LIST_DATABASES_H

// https://github.com/mongodb/specifications/blob/master/source/enumerate-databases.rst

#include "Op_Query.h"
#include "Op_Reply.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/MongoUtility.h"
#include "ThorSerialize/JsonPrinter.h"
#include <string>

namespace ThorsAnvil::DB::Mongo
{

struct AdmCmdListDataBases
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
    std::vector<DBInfo> databases;
    double              totalSize;
};

class CmdAdmListDataBases: public Op_Query<AdmCmdListDataBases>
{
    public:
        CmdAdmListDataBases()
            : Op_Query("admin.$cmd", {})
        {}
        friend std::ostream& operator<<(std::ostream& stream, CmdAdmListDataBases const& data) {return stream << static_cast<Op_Query<AdmCmdListDataBases>>(data);}
};

using CmdAdmListDataBasesReply = Op_Reply<ListDataBaseReply>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::AdmCmdListDataBases,listDatabases);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::DBInfo,             name, sizeOnDisk, empty);
ThorsAnvil_MakeTrait(ThorsAnvil::DB::Mongo::ListDataBaseReply,  databases, totalSize);

#endif
