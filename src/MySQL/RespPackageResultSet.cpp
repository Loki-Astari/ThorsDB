
#include "RespPackageResultSet.h"
#include <cassert>

using namespace ThorsAnvil::MySQL::Detail;

RespPackageResultSet::RespPackageResultSet(
                            int firstByte,
                            ConectReader& reader,
                            std::vector<RespPackageColumnDefinition> const& columns
                        )
    : RespPackage(reader, "Result-Set")
    , reader(reader)
    , columns(columns)
    , nextColumn(0)
{
    assert(firstByte == 0x00);

    int nullmaplength   = (columns.size() + 7 + 2) / 8;
    nullMap = reader.fixedLengthString(nullmaplength);
}


