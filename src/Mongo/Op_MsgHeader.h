#ifndef THORSANVIL_DB_MONGO_MESSAGE_HEADER_H
#define THORSANVIL_DB_MONGO_MESSAGE_HEADER_H

#include "Op.h"         // Includes std::<integer>_t
#include "Util.h"

#include <iostream>

namespace ThorsAnvil::DB::Mongo
{

class Op_MsgHeader
{
    static std::int32_t     uniqueMessageId;

    std::int32_t            messageLength;          // total message size, including this
    std::int32_t            requestID;              // identifier for this message
    std::int32_t            responseTo;             // requestID from the original request (used in responses from db)
    OpCode                  opCode;                 // request type - see table below for details
    std::int8_t             compression;

    public:
        Op_MsgHeader(OpCode opCode = OpCode::OP_REPLY);
        void prepareToSend(std::size_t dataSize);
        std::int32_t getMessageLength() const           {return messageLength;}
        bool operator==(Op_MsgHeader const& rhs) const;
        bool operator!=(Op_MsgHeader const& rhs) const;

        std::ostream& print(std::ostream& stream) const;
        std::ostream& printHR(std::ostream& stream) const;
        std::istream& parse(std::istream& stream);

        bool        isCompressed() const                                                                   {return compression != 0;}
        void        setCompression(std::int8_t compressionType)                                            {compression = compressionType;}

        friend std::ostream& operator<<(std::ostream& stream, Op_MsgHeader const& header)                  {return header.print(stream);}
        friend std::istream& operator>>(std::istream& stream, Op_MsgHeader& header)                        {return header.parse(stream);}

        [[deprecated("This function is for unit testing only. Do not use it. I will make it do silly things in the future to mess with people that use this function")]]
        static void messageIdSetForTest(int value)                {uniqueMessageId = value;}  // Used for testing
};

}

#endif
