#ifndef THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H
#define THORSANVIL_MYSQL_PACKAGE_REQU_HANDSHAKE_RESPONSE_H

#include "RequPackage.h"
#include "ThorSQL/SQLUtil.h"
#include <sstream>

namespace ThorsAnvil
{
    namespace MySQL
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

inline std::ostream& RequPackageHandShakeResponse::print(std::ostream& s) const
{
    std::stringstream authRespDecoded;
    for (char x: authResponse)
    {   authRespDecoded << "0x" << std::hex << static_cast<unsigned int>(static_cast<unsigned char>(x)) << " ";
    }
    std::stringstream keyValDecoded;
    for (auto const& val: options)
    {   keyValDecoded << "KV(" << val.first << " => " << val.second << ") ";
    }

    return s << "HandshakeResponsePackage: "
             << "username(" << username << ") "
             << "authResponse(" << authRespDecoded.str() << ") "
             << "options(" << keyValDecoded.str() << ") "
             << "database(" << database << ") "
             << "authPluginName(" << authPluginName << ") "
             << "capabilities(" << capabilities << ") ";
}

    }
}

#endif
