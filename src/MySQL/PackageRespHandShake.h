#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_HAND_SHAKE_H

#include "PackageResp.h"
#include "PackageConReader.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

class PackageRespHandShake: public PackageResp
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
        PackageRespHandShake(PackageConReader& reader);
        virtual std::ostream& print(std::ostream& s) const;
        long                getCapabilities()   const                   {return capabilities;}
        long                getCharset()        const                   {return charset;}
};

        }
    }
}

#endif
