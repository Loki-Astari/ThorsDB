
#ifndef THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H
#define THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H

#include "PackageRequ.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

class PackageRespHandShake;
class PackageRequHandShakeResponse: public PackageRequ
{
    std::string username;
    std::string authResponse;
    std::string database;
    std::string authPluginName;
    long        capabilities;
    //std::map<std::string,std::string>   keyValueStore;

    public:
        PackageRequHandShakeResponse(std::string const& username,
                                 std::string const& password,
                                 std::string const& database,
                                 long capabilities,
                                 PackageRespHandShake const& handshake);

        virtual  std::ostream& print(std::ostream& s)   const override;
        virtual  void send(PackageConWriter& writer)    const override;
};

        }
    }
}

#endif

