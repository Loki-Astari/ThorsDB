#ifndef THORS_ANVIL_DB_MYSQL_REQU_PACKAGE_PREPARE_RESET_H
#define THORS_ANVIL_DB_MYSQL_REQU_PACKAGE_PREPARE_RESET_H

#include "ThorMySQL.h"
#include "RequPackage.h"
#include <ostream>

namespace ThorsAnvil::DB::MySQL
{

class RequPackagePrepareReset: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareReset(int statementID)
            : RequPackage("RequPackagePrepareReset", "Prepare-Reset")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const
        {
            return s << "RequPackagePrepareReset: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-reset.html#com-stmt-reset
            writer.writeFixedLengthInteger<1>(COM_STMT_RESET);
            writer.writeFixedLengthInteger<4>(statementID);
        }
};

inline void testPrintRequPackagePrepareReset(std::ostream& str)
{
    str << RequPackagePrepareReset(1);
}

}

#endif
