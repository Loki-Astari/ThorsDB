#ifndef THORS_ANVIL_DB_MYSQL_PACKAGE_REQU_AUTH_SWITCH_RESPONSE_H
#define THORS_ANVIL_DB_MYSQL_PACKAGE_REQU_AUTH_SWITCH_RESPONSE_H

#include "RequPackage.h"
#include "RespPackageAuthSwitchRequest.h"
#include <string>
#include <ostream>

namespace ThorsAnvil::DB::MySQL
{

using Options=Access::Options;
class RespPackageHandShake;
class ConectWriter;

class RequPackageAuthSwitchResponse: public RequPackage
{
    std::string        authResponse;

    public:
        RequPackageAuthSwitchResponse(std::string const& username,
                                 std::string const& password,
                                 Options const& options,
                                 std::string const& database,
                                 std::string const& authResponse);

        virtual  std::ostream& print(std::ostream& s)   const override;
        virtual  void build(ConectWriter& writer)       const override;
};

}

#endif
