
#ifndef THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H
#define THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H

#include "PackageRequ.h"
#include "ThorSQL/SQLUtil.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

using Options=SQL::Options;
class PackageRespHandShake;
class PackageRequHandShakeResponse: public PackageRequ
{
    std::string const& username;
    std::string        authResponse;
    Options const&     options;
    std::string const& database;
    std::string const& authPluginName;
    long               capabilities;

    public:
        PackageRequHandShakeResponse(std::string const& username,
                                 std::string const& password,
                                 Options const& options,
                                 std::string const& database,
                                 PackageRespHandShake const& handshake);

        virtual  std::ostream& print(std::ostream& s)   const override;
        virtual  void build(PackageConWriter& writer)   const override;
};

        }
    }
}

#endif

