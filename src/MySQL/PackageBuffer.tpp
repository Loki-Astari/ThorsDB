
#include <algorithm>
#include <arpa/inet.h>


template<typename T>
ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::PackageBufferMySQLDebugBuffer(T& stream)
    : stream(stream)
    , currentPacketSize(0)
    , currentPacketPosition(0)
    , currentPacketSequenceID(-1)
    , hasMore(true)
{}

template<typename T>
inline void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::read(char* buffer, std::size_t len)
{
    std::size_t retrieved = 0;
    do
    {
        std::size_t remaining      = currentPacketSize - currentPacketPosition;
        std::size_t getFromPacket  = std::min((len - retrieved), remaining);
        stream.read(buffer + retrieved, getFromPacket);
        retrieved               += getFromPacket;
        currentPacketPosition   += getFromPacket;
        if (retrieved != len && remaining == 0)
        {   nextPacket();
        }
    }
    while(len != retrieved);
}

template<typename T>
bool ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::isEmpty()
{
    long remaining      = currentPacketSize - currentPacketPosition;
    if ((remaining == 0) && hasMore)
    {
        nextPacket();
        remaining      = currentPacketSize - currentPacketPosition;
    }
    return remaining == 0;
}

template<typename T>
std::string ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::readRemainingData()
{
    std::string dst;
    do
    {
        long retrieved  = dst.size();
        long remaining  = currentPacketSize - currentPacketPosition;
        dst.resize(retrieved + remaining);
        read(&dst[retrieved], remaining);
        if (hasMore) {
            nextPacket();
            continue;
        }
        break;
    }
    while(true);

    return dst;
}

template<typename T>
void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::nextPacket()
{
    if (!hasMore)
    {   throw std::runtime_error("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer::nextPacket: No more data expected from server");
    }
    currentPacketSize       = 0;
    currentPacketPosition   = 0;
    currentPacketSequenceID++;

    std::uint32_t    packetBufferSize = 0;
    stream.read(reinterpret_cast<char*>(&packetBufferSize), 3);
    ntohl(currentPacketSize);
    currentPacketSize   = packetBufferSize;

    char actualSequenceID;
    stream.read(&actualSequenceID, 1);

    if (currentPacketSequenceID != actualSequenceID)
    {   throw std::runtime_error(std::string("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer::nextPacket: currentPacketSequenceID(") + std::to_string(currentPacketSequenceID) + ") != actual sequence on input stream(" + std::to_string(actualSequenceID) + ")");
    }

    hasMore = currentPacketSize == 0xFFFFFF;
}


