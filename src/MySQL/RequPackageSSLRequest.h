#ifndef THORS_ANVIL_DB_MYSQL_PACKAGE_REQU_SSL_REQUEST_H
#define THORS_ANVIL_DB_MYSQL_PACKAGE_REQU_SSL_REQUEST_H

#include "RequPackage.h"
#include "ThorsDB/SQLUtil.h"
#include <string>
#include <ostream>

namespace ThorsAnvil::DB::MySQL
{

using Options=Access::Options;
class RespPackageHandShake;
class ConectWriter;

class RequPackageSSLRequest: public RequPackage
{
    long               capabilities;

    public:
        RequPackageSSLRequest(long capabilities);

        virtual  std::ostream& print(std::ostream& s)   const override;
        virtual  void build(ConectWriter& writer)       const override;
        long getCapabilities()                          const {return capabilities;}
};

}

#endif
