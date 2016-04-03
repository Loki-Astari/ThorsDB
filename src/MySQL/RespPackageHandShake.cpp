
#include "RespPackageHandShake.h"
#include "ConectReader.h"
#include <cassert>

using namespace ThorsAnvil::MySQL::Detail;

RespPackageHandShake::RespPackageHandShake(int firstbyte, ConectReader& reader)
    : RespPackage(reader, "HandShake")
    , serverVersion(reader.nulTerminatedString())
    , connectionID(reader.fixedLengthInteger<4>())
    , authPluginData(reader.fixedLengthString(8))
    , check(0)
    , statusFlag(0)
    , authPluginLength(0)
    , isV9(false)
    , capabilities(0)
{
    assert(firstbyte = 0x0A);

    if (reader.isEmpty()) {
        isV9    = true;
        return;
    }
    check           = reader.fixedLengthInteger<1>();
    capabilities    = reader.fixedLengthInteger<2>();

    if (reader.isEmpty()) {
        return;
    }

    charset         = reader.fixedLengthInteger<1>();
    statusFlag      = reader.fixedLengthInteger<2>();
    long cap2       = reader.fixedLengthInteger<2>();
    authPluginLength= reader.fixedLengthInteger<1>();
    reserved        = reader.fixedLengthString(10);
    std::string authPluginData2 = reader.variableLengthString(std::max(13L, authPluginLength - 8L));
    authPluginName  = reader.nulTerminatedString();

    capabilities    = capabilities | (cap2 << 16);
    // Because of the way 13L is the min size for authPluginData2 data
    // This may exceed the actual size of the auth data. So after concatenating the
    // data make sure we only use the required length `authPluginLength`. This means
    // removing the extra terminating '\0' character.
    // see: mysql-5.7/sql/auth/sql_authentication.cc 538
    // the 13th byte is "\0 byte, terminating the second part of a scramble"
    authPluginData  = (authPluginData + authPluginData2).substr(0, authPluginLength-1);
}

