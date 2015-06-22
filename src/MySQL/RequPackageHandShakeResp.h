
#ifndef THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H
#define THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H

#include "RequPackage.h"
#include "ThorSQL/SQLUtil.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

using Options=SQL::Options;
class RespPackageHandShake;
class RequPackageHandShakeResponse: public RequPackage
{
    std::string const& username;
    std::string        authResponse;
    Options const&     options;
    std::string const& database;
    std::string const& authPluginName;
    long               capabilities;

    public:
        RequPackageHandShakeResponse(std::string const& username,
                                 std::string const& password,
                                 Options const& options,
                                 std::string const& database,
                                 RespPackageHandShake const& handshake);

        virtual  std::ostream& print(std::ostream& s)   const override;
        virtual  void build(ConectWriter& writer)       const override;
};

        }
    }
}

#endif

