#ifndef THORSANVIL_DB_MONGO_OP_MSG_H
#define THORSANVIL_DB_MONGO_OP_MSG_H

#include "Op.h"
#include "MsgHeader.h"
#include "/Users/martinyork/Repo/ThorsSerializer/src/Serialize/Traits.h"
#include "/Users/martinyork/Repo/ThorsSerializer/src/Serialize/BsonThor.h"
#include <ostream>
#include <istream>
#include <cstdlib>

namespace ThorsAnvil::DB::Mongo
{

enum class OP_MsgFlag: std::int32_t
{
    empty           = 0,
    checksumPresent = 1,                    // The message ends with 4 bytes containing a CRC-32C [1] checksum. See Checksum for details.
    moreToCome      = 2,                    // Another message will follow this one without further action from the receiver.
                                            // The receiver MUST NOT send another message until receiving one with moreToCome set to 0 as
                                            // sends may block, causing deadlock. Requests with the moreToCome bit set will not receive a reply.
                                            // Replies will only have this set in response to requests with the exhaustAllowed bit set.
    exhaustAllowed   = 0x10000              // The client is prepared for multiple replies to this request using the moreToCome bit.
                                            // The server will never produce replies with the moreToCome bit set unless the request has this bit set.
};

template<typename Data>
struct Kind0
{
    Data            data;
    public:
        template<typename... Args>
        Kind0(Args&&... arg);
        std::size_t getSize(std::ostream& stream);
        friend std::ostream& operator<<(std::ostream& stream, Kind0 const& msg);
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Kind0> const& msg);
};

template<typename... Kind>
class OP_Msg
{
    MsgHeader           header;             // standard message header
    OP_MsgFlag          flagBits;           // message flags
    std::tuple<Kind...> sections;           // The data of the message (See abive Kind0 and Kind1
    std::int32_t        checksum;           // checksum of message;

    public:
        template<typename... Args>
        OP_Msg(Args&&... arg);
        friend std::ostream& operator<<(std::ostream& stream, OP_Msg& msg);
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<OP_Msg>& msg);
};

}

#if defined(HEADER_ONLY) && HEADER_ONLY == 1
#include "Op_Msg.source"
#endif

#ifndef COVERAGE_TEST
#include "Op_Msg.tpp"
#endif

#endif
