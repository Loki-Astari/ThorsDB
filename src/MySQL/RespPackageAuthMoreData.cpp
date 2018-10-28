#include "ThorMySQL.h"
#include "ConectReader.h"
#include "RespPackageAuthMoreData.h"
#include <cassert>

using namespace ThorsAnvil::MySQL;

RespPackageAuthMoreData::RespPackageAuthMoreData(int firstbyte, ConectReader& reader)
    : RespPackage(RespType::Authentication, reader)
    , pluginMoreData(reader.restOfPacketString())
{
    // https://dev.mysql.com/doc/internals/en/connection-phase-packets.html#packet-Protocol::AuthMoreData
    assert(firstbyte = 0x01);
}
