#ifndef THORS_ANVIL_MYSQL_REQU_PACKAGE_PREPARE_CLOSE_H
#define THORS_ANVIL_MYSQL_REQU_PACKAGE_PREPARE_CLOSE_H

#include "ThorMySQL.h"
#include "ConectWriter.h"
#include "RequPackage.h"
#include <ostream>

namespace ThorsAnvil
{
    namespace MySQL
    {

class RequPackagePrepareClose: public RequPackage
{
    int statementID;
    public:
        RequPackagePrepareClose(int statementID)
            : RequPackage("RequPackagePrepareClose", "Prepare-Close")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const
        {
            return s << "RequPackagePrepareClose: statementID: " << statementID << "\n";
        }
        virtual  void build(ConectWriter& writer)       const
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-close.html#com-stmt-close
            writer.writeFixedLengthInteger<1>(COM_STMT_CLOSE);
            writer.writeFixedLengthInteger<4>(statementID);
        }
};

inline void testPrintRequPackagePrepareClose(std::ostream& str)
{
    str << RequPackagePrepareClose(1);
}

    }
}

#endif
