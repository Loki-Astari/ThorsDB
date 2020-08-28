#ifndef THORS_ANVIL_DB_MYSQL_RESP_PACKAGE_PREPARE_H
#define THORS_ANVIL_DB_MYSQL_RESP_PACKAGE_PREPARE_H

#include "RespPackage.h"
#include "RespPackageColumnDefinition.h"
#include <vector>
#include <ostream>

namespace ThorsAnvil::DB::MySQL
{

class ConectReader;

class RespPackagePrepare: public RespPackage
{
    int     statementID;
    int     numColumns;
    int     numParams;
    int     warningCount;
    std::vector<RespPackageColumnDefinition>   paramInfo;
    std::vector<RespPackageColumnDefinition>   columnInfo;
    public:
        RespPackagePrepare(int firstByte, ConectReader& reader);
        virtual std::ostream& print(std::ostream& s)    const override;

        bool    isSelect()                                            const {return numColumns != 0;}
        int     getStatementID()                                      const {return statementID;}
        std::vector<RespPackageColumnDefinition> const&  getColumns() const {return columnInfo;}
        std::vector<RespPackageColumnDefinition> const&  getParams()  const {return paramInfo;}
};

inline void testPrintRespPackagePrepare(std::ostream& str, int firstByte, ConectReader& reader)
{
    str << RespPackagePrepare(firstByte, reader);
}

}

#endif
