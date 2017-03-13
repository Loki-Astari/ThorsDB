#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_ERR_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_ERR_H

#include "RespPackage.h"
#include "ConectReader.h"
#include "ThorMySQL.h"
#include <cassert>

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

class RespPackageERR: public RespPackage
{
    long        errorCode;
    std::string hash;
    std::string stateSQL;
    public:
        RespPackageERR(int firstByte, ConectReader& reader)
            : RespPackage(reader, "ERR")
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
}

#endif
