#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_ERR_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_ERR_H

#include "PackageResp.h"
#include "PackageReader.h"
#include "ThorMySQL.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

class PackageRespERR: public PackageResp
{
    long        errorCode;
    std::string hash;
    std::string SQLState;
    public:
        PackageRespERR(PackageReader& reader)
            : PackageResp(reader)
            , errorCode(reader.fixedLengthInteger<2>())
            , hash(reader.fixedLengthString(1, CLIENT_PROTOCOL_41))
            , SQLState(reader.fixedLengthString(5, CLIENT_PROTOCOL_41))
        {
            error           = true;
            humanMessage    = reader.restOfPacketString();
        }
        virtual  std::ostream& print(std::ostream& s) const
        {
            return s << "ERRPackage: "
                     << "errorCode(" << errorCode << ") "
                     << "hash(" << hash << ") Should be '#' "
                     << "SQLState(" << SQLState << ") ";
        }
};

        }
    }
}

#endif
