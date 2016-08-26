#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H

#include "RespPackage.h"
#include <sstream>
#include <iomanip>

namespace ThorsAnvil
{
    namespace MySQL
    {
class ConectReader;
        namespace Detail
        {

class RespPackageHandShake: public RespPackage
{
    std::string     serverVersion;
    long            connectionID;
    std::string     authPluginData;
    long            check;
    //--
    long            statusFlag;
    long            authPluginLength;
    std::string     reserved;
    std::string     authPluginName;
    bool            isV9;
    long            capabilities;
    char            charset;

    public:
        RespPackageHandShake(int firstByte, ConectReader& reader);
        virtual std::ostream& print(std::ostream& s)    const;
        long                getCapabilities()           const   {return capabilities;}
        long                getCharset()                const   {return charset;}
        std::string const&  getAuthPluginName()         const   {return authPluginName;}
        std::string const&  getAuthPluginData()         const   {return authPluginData;}
};

inline std::ostream& RespPackageHandShake::print(std::ostream& s) const
{
    std::stringstream reservedDecoded;
    for (char x: reserved)
    {
        reservedDecoded << "0x" << std::hex << static_cast<int>(x) << " ";
    }
    std::stringstream authPluginDataDecoded;
    for (char x: authPluginData)
    {
        authPluginDataDecoded << "0x" << std::hex << static_cast<int>(x) << " ";
    }

    return s << "RespPackageHandShake: "
             << "serverVersion(" << serverVersion << ") "
             << "connectionID(" << connectionID << ") "
             << "authPluginData(" << authPluginDataDecoded.str() << ") "
             << "check(" << check << ") "
             << "statusFlag( 0x" << std::hex << std::setw(8) << std::setfill('0') << statusFlag << ") "
             << "authPluginLength(" << authPluginLength << ") "
             << "reserved("  << reservedDecoded.str() << ") "
             << "authPluginName(" << authPluginName << ") "
             << "capabilities(" << capabilities << ") "
             << "isV9(" << isV9 << ") "
             << std::dec;
}

        }
    }
}

#endif
