#include "ThorMySQL.h"
#include "Authentication.h"
#include "RequPackageAuthSwitchResp.h"
#include "RespPackageAuthSwitchRequest.h"

#include <iostream>

using namespace ThorsAnvil::DB::MySQL;

RequPackageAuthSwitchResponse::RequPackageAuthSwitchResponse(std::string const& /*username*/,
                                                           std::string const& /*password*/,
                                                           Options const&     /*options*/,
                                                           std::string const& /*database*/,
                                                           std::string const& authResponse)
    : RequPackage("RequPackageAuthSwitchResponse", "AuthSwitch-Response")
    , authResponse(authResponse)
{}

void RequPackageAuthSwitchResponse::build(ConectWriter& writer) const
{
    writer.writeVariableLengthString(authResponse);
}

std::ostream& RequPackageAuthSwitchResponse::print(std::ostream& s) const
{
    std::stringstream authRespDecoded;
    for (char x: authResponse)
    {   authRespDecoded << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(x)) << " ";
    }

    return s << "AuthSwitchResponsePackage: "
             << "authResponse(" << authRespDecoded.str() << ") ";
}
