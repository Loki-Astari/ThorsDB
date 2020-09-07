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
inline std::int32_t operator&(OP_MsgFlag const& lhs, OP_MsgFlag const& rhs)
{
    return static_cast<std::int32_t>(lhs) & static_cast<std::int32_t>(rhs);
}

template<typename Data>
struct Kind0
{
    Data            data;
    public:
        template<typename... Args>
        Kind0(Args&&... arg);
        std::size_t getSize(std::ostream& stream);

        std::ostream& print(std::ostream& stream)       const;
        std::ostream& printHR(std::ostream& stream)     const;

        friend std::ostream& operator<<(std::ostream& stream, Kind0 const& msg)                 {return msg.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Kind0> const& msg)  {return msg.object.printHR(stream);}
};

// TODO Kind1


// Op_Msg: https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg
template<typename... Kind>
class OP_Msg
{
    MsgHeader           header;             // standard message header
    OP_MsgFlag          flagBits;           // message flags
    std::tuple<Kind...> sections;           // The data of the message (See above Kind0 and Kind1)
    std::int32_t        checksum;           // checksum of message;

    public:
        template<typename... Args>
        OP_Msg(Args&&... arg);

        std::ostream& print(std::ostream& stream);
        std::ostream& printHR(std::ostream& stream);
    private:
        template<std::size_t... I> std::size_t getSectionSize(std::ostream& stream, std::index_sequence<I...>&&);
        template<std::size_t... I> void printSection(std::ostream& stream, std::index_sequence<I...>&&);
        template<std::size_t... I> void printSectionHR(std::ostream& stream, std::index_sequence<I...>&&);

        friend std::ostream& operator<<(std::ostream& stream, OP_Msg& msg)                  {return msg.print(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<OP_Msg>& msg)   {return msg.object.printHR(stream);}
};

}

#if defined(HEADER_ONLY) && HEADER_ONLY == 1
#include "Op_Msg.source"
#endif

#ifndef COVERAGE_TEST
#include "Op_Msg.tpp"
#endif

#endif
