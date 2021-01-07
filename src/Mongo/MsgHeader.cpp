#include "MsgHeader.h"

using namespace ThorsAnvil::DB::Mongo;

std::int32_t MsgHeader::uniqueMessageId = 0;

MsgHeader::MsgHeader(OpCode opCode)
    : messageLength(0)
    , requestID(0)
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

namespace ThorsAnvil::DB::Mongo
{
    std::ostream& operator<<(std::ostream& stream, MsgHeader& header)
    {
        return stream << make_LE(header.messageLength)
                      << make_LE(header.requestID)
                      << make_LE(header.responseTo)
                      << make_LE(header.opCode);
    }

    std::istream& operator>>(std::istream& stream, MsgHeader& header)
    {
        return stream >> make_LE(header.messageLength)
                      >> make_LE(header.requestID)
                      >> make_LE(header.responseTo)
                      >> make_LE(header.opCode);
    }

    std::ostream& operator<<(std::ostream& stream, HumanReadable<MsgHeader> const& header)
    {
        return stream << "messageLength: " << header.object.messageLength << "\n"
                      << "requestID:     " << header.object.requestID     << "\n"
                      << "responseTo:    " << header.object.responseTo    << "\n"
                      << "opCode:        " << static_cast<std::int32_t>(header.object.opCode) << "\n";
    }
}
