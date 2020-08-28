#ifndef THORS_ANVIL_MYSQL_REQU_PACKAGE_PREPARE_H
#define THORS_ANVIL_MYSQL_REQU_PACKAGE_PREPARE_H

#include "ThorMySQL.h"
#include "ConectWriter.h"
#include "RequPackage.h"
#include <ostream>
#include <string>

namespace ThorsAnvil::DB::MySQL
{

class RequPackagePrepare: public RequPackage
{
    std::string const& statement;
    public:
        RequPackagePrepare(std::string const& statement)
            : RequPackage("RequPackagePrepare", "Prepare")
            , statement(statement)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override
        {
            return s << "RequPackagePrepare: statement: " << statement << "\n";
        }
        virtual  void build(ConectWriter& writer)       const override
        {
            // https://dev.mysql.com/doc/internals/en/com-stmt-prepare.html#com-stmt-prepare
            writer.writeFixedLengthInteger<1>(COM_STMT_PREPARE);
            writer.writeVariableLengthString(statement);
        }
};

inline void testPrintRequPackagePrepare(std::ostream& str)
{
    str << RequPackagePrepare("Test");
}

}

#endif
