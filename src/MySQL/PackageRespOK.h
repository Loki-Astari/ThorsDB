#ifndef THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_OK_H
#define THORSANVIL_MYSQL_DETAILS_PACKAGE_RESP_OK_H

#include "PackageResp.h"
#include "PackageConReader.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

class PackageRespOK: public PackageResp
{
    long    affectedRows;
    long    lastInsertID;
    long    statusFlags;
    long    warningFlags;
    public:
        PackageRespOK(PackageConReader& reader)
            : PackageResp(reader)
            , affectedRows(reader.lengthEncodedInteger())
            , lastInsertID(reader.lengthEncodedInteger())
            , statusFlags(reader.fixedLengthInteger<2>())
            , warningFlags(reader.fixedLengthInteger<2>())
        {
            ok              = true;
            humanMessage    = reader.restOfPacketString();
        }
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
}

#endif
