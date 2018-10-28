#ifndef THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_ERR_H
#define THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_ERR_H

#include "ThorMySQL.h"
#include "ConectReader.h"
#include "RespPackage.h"
#include <string>
#include <ostream>
#include <cassert>

namespace ThorsAnvil
{
    namespace MySQL
    {

class RespPackageERR: public RespPackage
{
    long        errorCode;
    std::string hash;
    std::string stateSQL;
    public:
        RespPackageERR(int firstByte, ConectReader& reader)
            : RespPackage(RespType::Err, reader)
            , errorCode(reader.fixedLengthInteger<2>())
            , hash(reader.fixedLengthString(1, CLIENT_PROTOCOL_41))
            , stateSQL(reader.fixedLengthString(5, CLIENT_PROTOCOL_41))
        {
            // https://dev.mysql.com/doc/internals/en/packet-ERR_Packet.html#packet-ERR_Packet
            assert(firstByte == 0xFF);

            error           = true;
            humanMessage    = reader.restOfPacketString();
        }
        virtual  std::ostream& print(std::ostream& s) const
        {
            return s << "ERRPackage: "
                     << "errorCode(" << errorCode << ") "
                     << "hash(" << hash << ") Should be '#' "
                     << "stateSQL(" << stateSQL << ") ";
        }
};

    }
}

#endif
