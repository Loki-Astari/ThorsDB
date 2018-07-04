#include "ThorMySQL.h"
#include "Authentication.h"
#include "RequPackageAuthSwitchResp.h"
#include "RespPackageAuthSwitchRequest.h"

#include <iostream>

using namespace ThorsAnvil::MySQL;

RequPackageAuthSwitchResponse::RequPackageAuthSwitchResponse(std::string const& username,
                                                           std::string const& password,
                                                           Options const&     options,
                                                           std::string const& database,
                                                           RespPackageAuthSwitchRequest const& handshake)
    : RequPackage("RequPackageAuthSwitchResponse", "AuthSwitch-Response")
{
    authResponse = authentication(handshake.pluginName, handshake.pluginData, username, password, options, database);
}

void RequPackageAuthSwitchResponse::build(ConectWriter& writer) const
{
    writer.writeVariableLengthString(authResponse);
}

std::ostream& RequPackageAuthSwitchResponse::print(std::ostream& s) const
{
    std::stringstream authRespDecoded;
    for (char x: authResponse)
    {   authRespDecoded << "0x" << std::hex << static_cast<unsigned int>(static_cast<unsigned char>(x)) << " ";
    }

    return s << "AuthSwitchResponsePackage: "
             << "authResponse(" << authRespDecoded.str() << ") ";
}
