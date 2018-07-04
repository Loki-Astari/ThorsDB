#ifndef THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_EOF_H
#define THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_EOF_H

#include "RespPackage.h"
#include "ConectReader.h"
#include <string>
#include <ostream>
#include <iomanip>
#include <cassert>

namespace ThorsAnvil
{
    namespace MySQL
    {

class ConectReader;

class RespPackageAuthSwitchRequest: public RespPackage
{
    public:
        std::string pluginName;
        std::string pluginData;

        RespPackageAuthSwitchRequest(int firstByte, ConectReader& reader)
            : RespPackage(reader, "AuthSwitchRequest")
            , pluginName(reader.nulTerminatedString())
            , pluginData(reader.restOfPacketString())
        {
            // https://dev.mysql.com/doc/internals/en/connection-phase-packets.html#packet-Protocol::AuthSwitchRequest
            assert(firstByte == 0xFE);

            // The packet contains the terminating null as part of the string.
            // This is not actual part of the string so we strip it here.
            if (!pluginData.empty())
            {
                pluginData.pop_back();
            }
        }
        virtual  std::ostream& print(std::ostream& s) const
        {
            s << "AuthSwitchRequest: "
              << "pluginName(" << pluginName << ") "
              << "pluginData(";
            for (auto c: pluginData)
            {
                s << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c) << " ";
            }
            s << ") " << std::dec;
            return s;
        }
};

    }
}

#endif
