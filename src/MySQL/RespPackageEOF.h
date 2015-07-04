#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_EOF_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_EOF_H

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

class RespPackageEOF: public RespPackage
{
    long    warningCount;
    long    statusFlag;
    public:
        RespPackageEOF(int firstByte, ConectReader& reader)
            : RespPackage(reader)
            , warningCount(reader.fixedLengthInteger<2>(CLIENT_PROTOCOL_41))
            , statusFlag(reader.fixedLengthInteger<2>(CLIENT_PROTOCOL_41))
        {
            assert(firstByte == 0xFE);
            eof = true;
        }
        virtual  std::ostream& print(std::ostream& s) const
        {
            return s << "EOFPackage: "
                     << "warningCount( 0x" << std::hex << std::setw(8) << std::setfill('0') << warningCount << ") "
                     << "statusFlag( 0x"   << std::hex << std::setw(8) << std::setfill('0') << statusFlag << ") "
                     << std::dec;
        }

        long    getStatusFlag() const {return statusFlag;}
};

        }
    }
}

#endif
