#include "PackageBuffer.h"
#include "ThorSQL/SQLUtil.h"
#include <iomanip>
#include <algorithm>
#include <arpa/inet.h>
#include <iostream>

using namespace ThorsAnvil::Postgres;

PackageBuffer::PackageBuffer(SQL::StreamInterface& stream)
    : stream(stream)
    , messageOpen(false)
    , messageSize(0)
    , messageRead(0)
{
    sendBuffer.reserve(0xFFFFFF/*16MByte*/);
}

char PackageBuffer::getMessage()
{
    if (!isEmpty())
    {
        throw std::domain_error(bugReport("ThorsAnvil::Postgres::PackageBuffer::getMessage: Called while processing another message"));
    }

    return openMessage();
}

void PackageBuffer::read(char* buffer, std::size_t len)
{
    if (messageRead + len > messageSize)
    {
        throw std::domain_error(bugReport("ThorsAnvil::Postgres::PackageBuffer::read: read passed the end of a message"));
    }
    stream.read(buffer, len);
    messageRead += len;
}

bool PackageBuffer::isEmpty()
{
    return messageSize == messageRead;
}

void PackageBuffer::startNewConversation()
{}

void PackageBuffer::drop()
{
    char buffer[1024];
    while (!isEmpty())
    {
        std::size_t toRead = std::min(1024UL, (messageSize - messageRead));
        read(buffer, toRead);
    }
}

std::string PackageBuffer::readRemainingData()
{
    std::size_t size = messageSize - messageRead;
    std::string buffer(size, ' ');
    read(&buffer[0], size);
    return buffer;
}

void PackageBuffer::reset()
{
    drop();
    messageOpen = false;
    messageSize = 0;
    messageRead = 0;
}

char PackageBuffer::openMessage()
{
    char messageType;
    stream.read(&messageType, 1);
    std::uint32_t   size;
    stream.read(reinterpret_cast<char*>(&size), 4);
    messageSize = ntohl(size);
    messageRead = 4;
    messageOpen = true;

    return messageType;
}

void PackageBuffer::write(char const* buffer, std::size_t len)
{
    std::copy(buffer, buffer + len, std::back_inserter(sendBuffer));
}

void PackageBuffer::sendMessage(char type)
{
    if (type != '\0')
    {
        // Special case for the first hand-shake message sent to the server
        std::cout << "Send Message: " << type << "\n";
        stream.write(&type, 1);
    }
    // Add four for the size information
    std::uint32_t  size = htonl(sendBuffer.size() + 4);
    for(int loop = 0;loop < 4;++loop)
    {
        std::cout << "  S: " << loop << " : " << std::dec << ((int)*(reinterpret_cast<char*>(&size) + loop)) << " : " << std::hex << ((int)*(reinterpret_cast<char*>(&size) + loop)) << "\n";
    }
    stream.write(reinterpret_cast<char*>(&size), 4);
    flush();
}

void PackageBuffer::flush()
{
    for(std::size_t loop = 0 ;loop < sendBuffer.size(); ++loop)
    {
        std::cout << "  B: " << loop << " : " << std::setfill(' ') << std::setw(3) << std::dec << ((int)sendBuffer[loop]) << " : 0x" << std::setfill('0') << std::setw(2) << std::hex << ((int)sendBuffer[loop]) << " : " << sendBuffer[loop] << "\n";
    }
    stream.write(&sendBuffer[0], sendBuffer.size());
    sendBuffer.clear();
}
