#ifndef THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_OK_H
#define THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_OK_H

#include "ConectReader.h"
#include "RespPackage.h"
#include "ThorMySQL.h"
#include <ostream>
#include <iomanip>
#include <cassert>

namespace ThorsAnvil::DB::MySQL
{

class RespPackageOK: public RespPackage
{
    long    affectedRows;
    long    lastInsertID;
    long    statusFlags;
    long    warningFlags;
    public:
        RespPackageOK(int firstByte, ConectReader& reader)
            : RespPackage(RespType::Ok, reader)
            , affectedRows(reader.lengthEncodedInteger())
            , lastInsertID(reader.lengthEncodedInteger())
            // https://dev.mysql.com/doc/internals/en/status-flags.html
            , statusFlags(0)
            , warningFlags(0)
        {
            // https://dev.mysql.com/doc/internals/en/packet-OK_Packet.html#packet-OK_Packet
            assert(firstByte == 0x00);

            if (hasCap(CLIENT_PROTOCOL_41))
            {
                statusFlags     = reader.fixedLengthInteger<2>();
                warningFlags    = reader.fixedLengthInteger<2>();
            }
            else if (hasCap(CLIENT_TRANSACTIONS))
            {
                statusFlags     = reader.fixedLengthInteger<2>();
            }

            if (hasCap(CLIENT_SESSION_TRACK))
            {
                humanMessage    = reader.lengthEncodedString();
                if (hasCap(SERVER_SESSION_STATE_CHANGED))
                {
                    std::string stateChangeInfo = reader.lengthEncodedString();
                    // TODO
                    // Decode Session State Information
                    // See: https://dev.mysql.com/doc/internals/en/packet-OK_Packet.html#cs-packet-ok-sessioninfo-data
                    // Once done this can be enabled in RequPackageHandShakeResp.cpp (look for the `cap` variable)
                }
            }
            else
            {
                humanMessage    = reader.restOfPacketString();
            }
            ok              = true;
        }

        long getAffectedRows() const {return affectedRows;}
        long getLastInsertID() const {return lastInsertID;}

        virtual  std::ostream& print(std::ostream& s) const
        {
            return s << "OKPackage: "
                     << "affectedRows( 0x" << std::hex << std::setw(8) << std::setfill('0') << affectedRows << ") "
                     << "lastInsertID( 0x" << std::hex << std::setw(8) << std::setfill('0') << lastInsertID << ") "
                     << "statusFlags( 0x"  << std::hex << std::setw(8) << std::setfill('0') << statusFlags << ") "
                     << "warningFlags( 0x" << std::hex << std::setw(8) << std::setfill('0') << warningFlags << ") "
                     << std::dec;
        }
};

}

#endif
