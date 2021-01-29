#ifndef THORSANVIL_DB_MONGO_OP_MSG_H
#define THORSANVIL_DB_MONGO_OP_MSG_H

#include "Op.h"
#include "Op_MsgHeader.h"
#include "ThorSerialize/Traits.h"

#include <iostream>

// Op_Msg: https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg

namespace ThorsAnvil::DB::Mongo
{

enum class OP_MsgFlag: std::int32_t
{
    empty           = 0,
    checksumPresent = 1 << 0,               // The message ends with 4 bytes containing a CRC-32C [1] checksum. See Checksum for details.
    moreToCome      = 1 << 1,               // Another message will follow this one without further action from the receiver.
                                            // The receiver MUST NOT send another message until receiving one with moreToCome set to 0 as
                                            // sends may block, causing deadlock. Requests with the moreToCome bit set will not receive a reply.
                                            // Replies will only have this set in response to requests with the exhaustAllowed bit set.
    exhaustAllowed  = 1 << 16,              // The client is prepared for multiple replies to this request using the moreToCome bit.
                                            // The server will never produce replies with the moreToCome bit set unless the request has this bit set.
};
ThorsAnvil_MakeEnumFlag(OP_MsgFlag, empty, checksumPresent, moreToCome, exhaustAllowed);

template<typename Data>
struct Kind0
{
    Data        data;
    public:
        template<typename... Args>
        Kind0(Args&&... args);
        std::size_t getSize()                           const;

        Data&       getDocument()       {return data;}
        Data const& getDocument() const {return data;}

    private:
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Kind0> const& msg);
        friend std::ostream& operator<<(std::ostream& stream, Kind0 const& msg)                 {return msg.print(stream);}
        friend std::istream& operator>>(std::istream& stream, Kind0& msg)                       {return msg.parse(stream);}
        std::istream& parse(std::istream& stream);
        std::ostream& print(std::ostream& stream)       const;
        std::ostream& printHR(std::ostream& stream)     const;

};

// TODO Kind1

template<typename... Kind>
class Op_Msg
{
    Op_MsgHeader            header;             // standard message header
    OP_MsgFlag              flags;              // Flags
    std::tuple<Kind...>     sections;           // The data of the message (See above Kind0 and Kind1)
    std::int32_t            checksum;           // checksum of message;

    public:
        template<typename... Args>
        Op_Msg(Args&&... args);
        template<typename... Args>
        Op_Msg(OP_MsgFlag flags, Args&&... args);

        template<std::size_t I>
        auto&       getDocument()       {return std::get<I>(sections).getDocument();}
        template<std::size_t I>
        auto const& getDocument() const {return std::get<I>(sections).getDocument();}

    private:
        std::size_t   getSize()                     const;

        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<Op_Msg> const& msg);
        friend std::ostream& operator<<(std::ostream& stream, Op_Msg const& msg)           {return msg.print(stream);}
        friend std::istream& operator>>(std::istream& stream, Op_Msg& msg)                 {return msg.parse(stream);}
        std::ostream& print(std::ostream& stream)       const;
        std::ostream& printHR(std::ostream& stream)     const;
        std::istream& parse(std::istream& stream);
};

template<typename... Kind>
class Op_MsgReply: public Op_Msg<Kind...>
{
    public:
        template<typename... Args>
        Op_MsgReply(Args&&... args);
};

}

#include "Op_Msg.tpp"

#endif
