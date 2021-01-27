#include "Op_MsgHeader.h"

using namespace ThorsAnvil::DB::Mongo;

std::int32_t Op_MsgHeader::uniqueMessageId = 0;

Op_MsgHeader::Op_MsgHeader(OpCode opCode)
    : messageLength(0)
    , requestID(-1)//uniqueMessageId++)
    , responseTo(0)
    , opCode(opCode)
{}

void Op_MsgHeader::prepareToSend(std::size_t dataSize)
{
    messageLength = sizeof(messageLength) + sizeof(requestID) + sizeof(responseTo) + sizeof(opCode) + dataSize;
    requestID = uniqueMessageId++;
}

bool Op_MsgHeader::operator==(Op_MsgHeader const& rhs) const
{
    return std::tie(messageLength, requestID, responseTo, opCode)
        == std::tie(rhs.messageLength, rhs.requestID, rhs.responseTo, rhs.opCode);
}
bool Op_MsgHeader::operator!=(Op_MsgHeader const& rhs) const
{
    return !((*this) == rhs);
}

std::ostream& Op_MsgHeader::print(std::ostream& stream) const
{
    return stream << make_LE(messageLength)
                  << make_LE(requestID)
                  << make_LE(responseTo)
                  << make_LE(opCode);
}

std::istream& Op_MsgHeader::parse(std::istream& stream)
{
    return stream >> make_LE(messageLength)
                  >> make_LE(requestID)
                  >> make_LE(responseTo)
                  >> make_LE(opCode);
}

std::ostream& Op_MsgHeader::printHR(std::ostream& stream) const
{
    return stream << "messageLength: " << messageLength << "\n"
                  << "requestID:     " << requestID     << "\n"
                  << "responseTo:    " << responseTo    << "\n"
                  << "opCode:        " << static_cast<std::int32_t>(opCode) << "\n";
}
