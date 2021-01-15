#include "PackageBuffer.h"
#include "ThorsIOUtil/Utility.h"
#include "ThorsLogging/ThorsLogging.h"
#include <iomanip>
#include <algorithm>
#include <arpa/inet.h>
#include <iostream>

using namespace ThorsAnvil::DB::Postgres;
using ThorsAnvil::Utility::buildErrorMessage;
using ThorsAnvil::Utility::buildBugReport;

PackageBuffer::PackageBuffer(Common::StreamInterface& stream)
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
        ThorsLogAndThrowCritical("ThorsAnvil::DB::Postgres::PackageBuffer",
                                 "getMessage",
                                 "Called while processing another message");
    }

    return openMessage();
}

void PackageBuffer::read(char* buffer, std::size_t len)
{
    if (messageRead + len > messageSize)
    {
        ThorsLogAndThrowCritical("ThorsAnvil::DB::Postgres::PackageBuffer",
                                 "read",
                                 "read passed the end of a message");
    }
    stream.read(buffer, len);
    messageRead += len;
}

bool PackageBuffer::isEmpty()
{
    return messageSize == messageRead;
}

void PackageBuffer::startNewConversation(bool)
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

void PackageBuffer::establishSSLConnection()
{
    stream.establishSSLConnection();
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
        stream.write(&type, 1);
    }
    // Add four for the size information
    std::uint32_t  size = htonl(sendBuffer.size() + 4);
    stream.write(reinterpret_cast<char*>(&size), 4);
    flush();
}

void PackageBuffer::flush()
{
    stream.write(&sendBuffer[0], sendBuffer.size());
    sendBuffer.clear();
}
