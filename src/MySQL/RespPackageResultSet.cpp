
#include "RespPackageResultSet.h"
#include <assert.h>

using namespace ThorsAnvil::MySQL::Detail;

RespPackageResultSet::RespPackageResultSet(int firstByte, ConectReader& reader, std::vector<RespPackageColumnDefinition> const& columns)
    : RespPackage(reader)
    , reader(reader)
    , columns(columns)
    , nextColumn(0)
{
    std::cerr << "RespPackageResultSet\n";
    assert(firstByte == 0x00);

    int nullmaplength   = (columns.size() + 7 + 2) / 8;
    nullMap = reader.fixedLengthString(nullmaplength);
}


