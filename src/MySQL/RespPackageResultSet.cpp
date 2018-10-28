#include "RespPackageResultSet.h"
#include <cassert>

using namespace ThorsAnvil::MySQL;

RespPackageResultSet::RespPackageResultSet(
                            int firstByte,
                            ConectReader& reader,
                            std::vector<RespPackageColumnDefinition> const& columns
                        )
    : RespPackage(RespType::ResultSet, reader)
    , reader(reader)
    , columns(columns)
    , nextColumn(0)
{
    // https://dev.mysql.com/doc/internals/en/binary-protocol-resultset-row.html#binary-protocol-resultset-row
    assert(firstByte == 0x00);

    int nullmaplength   = (columns.size() + 7 + 2) / 8;
    nullMap = reader.fixedLengthString(nullmaplength);
}
