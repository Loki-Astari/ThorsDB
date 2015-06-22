#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H

#include "RespPackage.h"
#include "ConectReader.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
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
        RespPackageHandShake(ConectReader& reader);
        virtual std::ostream& print(std::ostream& s)    const;
        long                getCapabilities()           const   {return capabilities;}
        long                getCharset()                const   {return charset;}
        std::string const&  getAuthPluginName()         const   {return authPluginName;}
        std::string const&  getAuthPluginData()         const   {return authPluginData;}
};

        }
    }
}

#endif
