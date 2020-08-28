#ifndef THORS_ANVIL_DB_MYSQL_RESP_PACKAGE_PREPARE_EXECUTE_H
#define THORS_ANVIL_DB_MYSQL_RESP_PACKAGE_PREPARE_EXECUTE_H

#include "ConectReader.h"
#include "RespPackage.h"
#include "RespPackageEOF.h"
#include "RespPackagePrepare.h"
#include "RespPackageColumnDefinition.h"
#include <vector>
#include <ostream>

namespace ThorsAnvil::DB::MySQL
{

class ConectReader;
class RespPackagePrepare;

class RespPackagePrepareExecute: public RespPackage
{
    int  columnCount;
    bool hasRows;
    std::vector<RespPackageColumnDefinition>   columnInfo;
    public:
        RespPackagePrepareExecute(int firstByte, ConectReader& reader, RespPackagePrepare& prepareResp);
        virtual  std::ostream& print(std::ostream& s)   const override;

        int  getColumnCount() const {return columnCount;}
        bool hasDataRows()    const {return hasRows;}
        std::vector<RespPackageColumnDefinition> const&  getColumns() const {return columnInfo;}
};

inline void testPrintRespPackagePrepareExecute(std::ostream& str, int firstBytePrep, int firstByteExec, ConectReader& reader)
{
    RespPackagePrepare  prep(firstBytePrep, reader);
    str << RespPackagePrepareExecute(firstByteExec, reader, prep);
}

}

#endif
