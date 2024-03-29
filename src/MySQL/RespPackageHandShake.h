#ifndef THORSANVIL_DB_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H
#define THORSANVIL_DB_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H

#include "RespPackage.h"
#include <string>
#include <sstream>
#include <ostream>
#include <iomanip>

namespace ThorsAnvil::DB::MySQL
{

class ConectReader;

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
    for (unsigned char x: reserved)
    {
        reservedDecoded << "0x" << std::hex << static_cast<int>(x) << " ";
    }
    std::stringstream authPluginDataDecoded;
    for (unsigned char x: authPluginData)
    {
        authPluginDataDecoded << "0x" << std::hex << static_cast<int>(x) << " ";
    }

    return s << "RespPackageHandShake: "
             << "serverVersion(" << serverVersion << ") "
             << "connectionID(0x" << std::setw(8) << std::setfill('0') << std::hex << connectionID << ") "
             << "authPluginData(" << authPluginDataDecoded.str() << ") "
             << "check(" << std::hex << check << ") "
             << "statusFlag( 0x" << std::hex << std::setw(8) << std::setfill('0') << statusFlag << ") "
             << "authPluginLength(" << std::dec << authPluginLength << ") "
             << "reserved("  << reservedDecoded.str() << ") "
             << "authPluginName(" << authPluginName << ") "
             << "capabilities(0x" << std::setw(8) << std::setfill('0') << std::hex << capabilities << ") "
             << "isV9(" << isV9 << ") "
             << std::dec;
}

}

#endif
