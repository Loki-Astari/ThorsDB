#include "MsgHeader.h"

using namespace ThorsAnvil::DB::Mongo;

std::int32_t MsgHeader::uniqueMessageId = 0;

MsgHeader::MsgHeader(OpCode opCode)
    : messageLength(0)
    , requestID(uniqueMessageId++)
    , responseTo(0)
    , opCode(opCode)
{}

void MsgHeader::prepareToSend(std::size_t dataSize)
{
    messageLength = sizeof(messageLength) + sizeof(requestID) + sizeof(responseTo) + sizeof(opCode) + dataSize;
    requestID = uniqueMessageId++;
}

bool MsgHeader::operator==(MsgHeader const& rhs) const
{
    return std::tie(messageLength, requestID, responseTo, opCode)
        == std::tie(rhs.messageLength, rhs.requestID, rhs.responseTo, rhs.opCode);
}
bool MsgHeader::operator!=(MsgHeader const& rhs) const
{
    return !((*this) == rhs);
}

std::ostream& MsgHeader::print(std::ostream& stream) const
{
    return stream << make_LE(messageLength)
                  << make_LE(requestID)
                  << make_LE(responseTo)
                  << make_LE(opCode);
}

std::istream& MsgHeader::parse(std::istream& stream)
{
    return stream >> make_LE(messageLength)
                  >> make_LE(requestID)
                  >> make_LE(responseTo)
                  >> make_LE(opCode);
}

std::ostream& MsgHeader::printHR(std::ostream& stream) const
{
    return stream << "messageLength: " << messageLength << "\n"
                  << "requestID:     " << requestID     << "\n"
                  << "responseTo:    " << responseTo    << "\n"
                  << "opCode:        " << static_cast<std::int32_t>(opCode) << "\n";
}
