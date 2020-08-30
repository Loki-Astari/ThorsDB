#ifndef THORS_ANVIL_DB_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H
#define THORS_ANVIL_DB_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H

#include "RequPackage.h"
#include "ThorsDB/SQLUtil.h"
#include <string>
#include <ostream>

namespace ThorsAnvil::DB::MySQL
{

using Options=Access::Options;
class RespPackageHandShake;
class ConectWriter;

class RequPackageHandShakeResponse: public RequPackage
{
    std::string const& username;
    std::string        authResponse;
    Options const&     options;
    std::string const& database;
    std::string const  authPluginName;
    long               capabilities;

    public:
        RequPackageHandShakeResponse(std::string const& username,
                                 std::string const& password,
                                 Options const& options,
                                 std::string const& database,
                                 long capabilities,
                                 std::string const& authPluginName,
                                 std::string const& authResponse);

        virtual  std::ostream& print(std::ostream& s)   const override;
        virtual  void build(ConectWriter& writer)       const override;
        long getCapabilities()                          const {return capabilities;}
};

}

#endif
