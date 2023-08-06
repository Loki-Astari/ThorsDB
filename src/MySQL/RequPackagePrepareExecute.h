#ifndef THORSANVIL_DB_MYSQL_REQU_PACKAGE_PREPARE_EXECUTE_H
#define THORSANVIL_DB_MYSQL_REQU_PACKAGE_PREPARE_EXECUTE_H

#include "RequPackage.h"
#include "BindBuffer.h"
#include <ostream>
#include <vector>

namespace ThorsAnvil::DB::MySQL
{

class RequPackagePrepareExecute: public RequPackage
{
    int                 statementID;
    BindBuffer const&   bindBuffer;
    public:
        RequPackagePrepareExecute(int statementID, BindBuffer const& bindBuffer)
            : RequPackage("RequPackagePrepareExecute", "Prepare-Execute")
            , statementID(statementID)
            , bindBuffer(bindBuffer)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override
        {
            return s << "RequPackagePrepareExecute: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const override
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-execute.html#com-stmt-execute
            writer.writeFixedLengthInteger<1>(COM_STMT_EXECUTE);
            writer.writeFixedLengthInteger<4>(statementID);
            /*
             *  0x00    CURSOR_TYPE_NO_CURSOR
             *  0x01    CURSOR_TYPE_READ_ONLY
             *  0x02    CURSOR_TYPE_FOR_UPDATE
             *  0x04    CURSOR_TYPE_SCROLLABLE
             */
            writer.writeFixedLengthInteger<1>(0x00);
            writer.writeFixedLengthInteger<4>(1);
            bindBuffer.bindToMySQL(writer);
        }
};

inline void testPrintRequPackagePrepareExecute(std::ostream& str)
{
    std::vector<RespPackageColumnDefinition>    cols;
    BindBuffer  bindBuffer(cols);
    str << RequPackagePrepareExecute(1, bindBuffer);
}

}

#endif
