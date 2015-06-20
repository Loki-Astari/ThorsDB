
#include <algorithm>
#include <arpa/inet.h>


template<typename T>
ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::PackageBufferMySQLDebugBuffer(T& stream)
    : stream(stream)
    , readCurrentPacketSize(0)
    , readCurrentPacketPosition(0)
    , readCurrentPacketSequenceID(-1)
    , writCurrentPacketSequenceID(-1)
    , hasMore(true)
{
    sendBuffer.reserve(0xFFFFFF/*16MByte*/);
}

template<typename T>
inline void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::read(char* buffer, std::size_t len)
{
    std::size_t retrieved = 0;
    do
    {
        std::size_t remaining      = readCurrentPacketSize - readCurrentPacketPosition;
        std::size_t getFromPacket  = std::min((len - retrieved), remaining);
        stream.read(buffer + retrieved, getFromPacket);
        retrieved               += getFromPacket;
        readCurrentPacketPosition   += getFromPacket;
        if (retrieved != len && remaining == 0)
        {   nextPacket();
        }
    }
    while(len != retrieved);
}
 
template<typename T>
bool ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::isEmpty()
{
    long remaining      = readCurrentPacketSize - readCurrentPacketPosition;
    if ((remaining == 0) && hasMore)
    {
        nextPacket();
        remaining      = readCurrentPacketSize - readCurrentPacketPosition;
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
        long remaining  = readCurrentPacketSize - readCurrentPacketPosition;
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
    readCurrentPacketSize       = 0;
    readCurrentPacketPosition   = 0;
    readCurrentPacketSequenceID++;

    std::uint32_t    packetBufferSize = 0;
    stream.read(reinterpret_cast<char*>(&packetBufferSize), 3);
    ntohl(readCurrentPacketSize);
    readCurrentPacketSize   = packetBufferSize;

    char actualSequenceID;
    stream.read(&actualSequenceID, 1);

    if (readCurrentPacketSequenceID != actualSequenceID)
    {   throw std::runtime_error(std::string("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer::nextPacket: readCurrentPacketSequenceID(") + std::to_string(readCurrentPacketSequenceID) + ") != actual sequence on input stream(" + std::to_string(actualSequenceID) + ")");
    }

    hasMore = readCurrentPacketSize == 0xFFFFFF;
}

template<typename T>
void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::write(char const* buffer, std::size_t len)
{
    std::size_t   currentSize = sendBuffer.size();
    if (currentSize + len > 0xFFFFFF)
    {
        std::size_t   available   = 0xFFFFFF - currentSize;
        writePackageHeader(0xFFFFFF);
        writeStream(&sendBuffer[0], currentSize);
        writeStream(buffer, available);
        sendBuffer.clear();

        buffer  += available;
        len     -= available;

        while (len > 0xFFFFFF)
        {
            writePackageHeader(0xFFFFFF);
            writeStream(buffer, 0xFFFFFF);

            buffer  += 0xFFFFFF;
            len     -= 0xFFFFFF;
        }
    }
    sendBuffer.insert(sendBuffer.end(), buffer, buffer + len);
}

template<typename T>
void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::flush()
{
    std::size_t   currentSize = sendBuffer.size();

    writePackageHeader(currentSize);
    writeStream(&sendBuffer[0], currentSize);
    sendBuffer.clear();
    writCurrentPacketSequenceID = -1;
}

template<typename T>
void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::writePackageHeader(std::size_t size)
{
    ++writCurrentPacketSequenceID;
    stream.write(reinterpret_cast<char const*>(&size), 3);
    stream.write(reinterpret_cast<char const*>(&writCurrentPacketSequenceID), 1);
}

template<typename T>
void ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::writeStream(char const* buffer, std::size_t len)
{
    stream.write(buffer, len);
}
 
