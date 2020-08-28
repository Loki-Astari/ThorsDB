#include "RespPackagePrepareExecute.h"

using namespace ThorsAnvil::DB:: MySQL;

RespPackagePrepareExecute::RespPackagePrepareExecute(int firstByte, ConectReader& reader, RespPackagePrepare& /*prepareResp*/)
    : RespPackage(RespType::PrepareExecute, reader)
{
    // https://dev.mysql.com/doc/internals/en/binary-protocol-resultset.html#binary-protocol-resultset
    columnCount = firstByte;
    reader.reset();
    for (int loop = 0;loop < columnCount; ++loop)
    {
        columnInfo.push_back(RespPackageColumnDefinition(reader));
        reader.reset();
    }
    auto mark = reader.recvMessageEOF();
    hasRows = !(mark->getStatusFlag() & SERVER_STATUS_CURSOR_EXISTS);

    // Stream now contains the data followed by an EOF token
}

std::ostream& RespPackagePrepareExecute::print(std::ostream& s) const
{
    return s << "RespPackagePrepareExecute: columnCount: " << columnCount << " hasRows: " << hasRows << "\n";
}
