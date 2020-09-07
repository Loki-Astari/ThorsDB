#ifndef THORSANVIL_DB_MONGO_MESSAGE_HEADER_H
#define THORSANVIL_DB_MONGO_MESSAGE_HEADER_H

#include "Op.h"

namespace ThorsAnvil::DB::Mongo
{

class MsgHeader
{
    static std::int32_t uniqueMessageId;

    std::int32_t    messageLength;          // total message size, including this
    std::int32_t    requestID;              // identifier for this message
    std::int32_t    responseTo;             // requestID from the original request (used in responses from db)
    OpCode          opCode;                 // request type - see table below for details
    public:
        MsgHeader(OpCode opCode = OpCode::OP_REPLY);
        void prepareToSend(std::size_t dataSize);
        std::int32_t getMessageLength() const       {return messageLength;}
        bool operator==(MsgHeader const& rhs) const;
        bool operator!=(MsgHeader const& rhs) const;

        std::ostream& print(std::ostream& stream);
        std::istream& parse(std::istream& stream);
        std::ostream& printHR(std::ostream& stream) const;

        friend std::ostream& operator<<(std::ostream& stream, MsgHeader& header)                        {return header.print(stream);}
        friend std::istream& operator>>(std::istream& stream, MsgHeader& header)                        {return header.parse(stream);}
        friend std::ostream& operator<<(std::ostream& stream, HumanReadable<MsgHeader> const& header)   {return header.object.printHR(stream);}


        [[deprecated("This function is for unit testing only. Do not use it. I will make it do silly things in the future to mess with people that use this function")]]
        static void messageIdSetForTest(int value)                {uniqueMessageId = value;}  // Used for testing
};

}

#endif
