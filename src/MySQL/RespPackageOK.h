#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_OK_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_OK_H

#include "RespPackage.h"
#include "ConectReader.h"
#include <cassert>
#include <iomanip>

namespace ThorsAnvil
{
    namespace MySQL
    {

class RespPackageOK: public RespPackage
{
    long    affectedRows;
    long    lastInsertID;
    long    statusFlags;
    long    warningFlags;
    public:
        RespPackageOK(int firstByte, ConectReader& reader)
            : RespPackage(reader, "OK")
            , affectedRows(reader.lengthEncodedInteger())
            , lastInsertID(reader.lengthEncodedInteger())
            // https://dev.mysql.com/doc/internals/en/status-flags.html
            , statusFlags(reader.fixedLengthInteger<2>())
            , warningFlags(reader.fixedLengthInteger<2>())
        {
            // https://dev.mysql.com/doc/internals/en/packet-OK_Packet.html#packet-OK_Packet
            assert(firstByte == 0x00);

            ok              = true;
            humanMessage    = reader.restOfPacketString();
        }

        long getAffectedRows() const {return affectedRows;}
        long getLastInsertID() const {return lastInsertID;}

        virtual  std::ostream& print(std::ostream& s) const
        {
            return s << "OKPackage: "
                     << "affectedRows(" << affectedRows << ") "
                     << "lastInsertID(" << lastInsertID << ") "
                     << "statusFlags( 0x"  << std::hex << std::setw(8) << std::setfill('0') << statusFlags << ") "
                     << "warningFlags( 0x" << std::hex << std::setw(8) << std::setfill('0') << warningFlags << ") "
                     << std::dec;
        }
};

    }
}

#endif
