#include "RespPackageEOF.h"
#include "RespPackagePrepare.h"
#include <cassert>

using namespace ThorsAnvil::DB::MySQL;

RespPackagePrepare::RespPackagePrepare(int firstByte, ConectReader& reader)
    : RespPackage(RespType::Prepare, reader)
{
    // https://dev.mysql.com/doc/internals/en/com-stmt-prepare-response.html#com-stmt-prepare-response
    assert(firstByte == 0x00);

    // Not We have already read 1 byte (status OK)
    statementID     = reader.fixedLengthInteger<4>();
    numColumns      = reader.fixedLengthInteger<2>();
    numParams       = reader.fixedLengthInteger<2>();
                      reader.fixedLengthInteger<1>(); // reserved
    warningCount    = reader.fixedLengthInteger<2>();

    if (numParams > 0)
    {
        reader.reset();
        for (int loop = 0;loop < numParams; ++loop)
        {
            paramInfo.emplace_back(reader);
            reader.reset();
        }

        reader.recvMessageEOF();
    }
    if (numColumns > 0)
    {
        reader.reset();
        for (int loop = 0;loop < numColumns; ++loop)
        {
            columnInfo.emplace_back(reader);
            reader.reset();
        }
        reader.recvMessageEOF();
    }
    /* To prevent exceptions in RespPackageResultSet when two many arguments are provided
     * We are going to push an extra fake column into the columns here.
     *
     * This is compensated for in Validation Stream which will detect the error
     * and generate a more appropriate exception that will happen during validation
     * rather than at runtime.
     */
     columnInfo.push_back(RespPackageColumnDefinition::getFakeColumn(MYSQL_TYPE_TINY));
}

std::ostream& RespPackagePrepare::print(std::ostream& s) const
{
    s << "RespPackagePrepare: statementID" << statementID
      << " numColumns: " << numColumns
      << " numParams: " << numParams
      << " warningCount: " << warningCount << "\n";
    s << "\tparamInfo:\n";
    for (auto const& loop: paramInfo)
    {
        s << "\t\t" << loop;
    }
    s << "\tcolumnInfo:\n";
    for (auto const& loop: columnInfo)
    {
        s << "\t\t" << loop;
    }
    return s;
}
