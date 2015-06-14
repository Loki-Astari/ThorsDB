
#include <algorithm>
#include <stdexcept>
#include <arpa/inet.h>

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename T>
PackageBufferMySQLDebugBuffer<T>::PackageBufferMySQLDebugBuffer(T& stream, bool flushed)
    : stream(stream)
    , readCurrentPacketSize(0)
    , readCurrentPacketPosition(0)
    , currentPacketSequenceID(-1)
    , hasMore(true)
    , flushed(flushed)
{
    sendBuffer.reserve(0xFFFFFF/*16MByte*/);
}

template<typename T>
inline void PackageBufferMySQLDebugBuffer<T>::read(char* buffer, std::size_t len)
{
    std::size_t retrieved = 0;
    do
    {
        std::size_t remaining      = readCurrentPacketSize - readCurrentPacketPosition;
        std::size_t getFromPacket  = std::min((len - retrieved), remaining);
        stream.read(buffer + retrieved, getFromPacket);
        retrieved                   += getFromPacket;
        readCurrentPacketPosition   += getFromPacket;
        if (retrieved != len && remaining == 0)
        {   nextPacket();
        }
    }
    while(len != retrieved);
}
 
template<typename T>
bool PackageBufferMySQLDebugBuffer<T>::isEmpty()
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
std::string PackageBufferMySQLDebugBuffer<T>::readRemainingData()
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
void PackageBufferMySQLDebugBuffer<T>::nextPacket()
{
    if (!hasMore)
    {   throw std::runtime_error("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer::nextPacket: No more data expected from server");
    }
    readCurrentPacketSize       = 0;
    readCurrentPacketPosition   = 0;
    currentPacketSequenceID++;

    std::uint32_t    packetBufferSize = 0;
    // TODO FIX only works on little endian
    stream.read(reinterpret_cast<char*>(&packetBufferSize), 3);
    readCurrentPacketSize   = packetBufferSize;

    char actualSequenceID;
    stream.read(&actualSequenceID, 1);

    if (currentPacketSequenceID != actualSequenceID)
    {   throw std::runtime_error(std::string("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer::nextPacket: currentPacketSequenceID(") + std::to_string(currentPacketSequenceID) + ") != actual sequence on input stream(" + std::to_string(actualSequenceID) + ")");
    }

    hasMore = readCurrentPacketSize == 0xFFFFFF;
}

template<typename T>
void PackageBufferMySQLDebugBuffer<T>::write(char const* buffer, std::size_t len)
{
    if (flushed) {
        throw std::runtime_error("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer::write: Writting to a flushed buffer");
    }
    std::size_t   currentSize = sendBuffer.size();
    if (currentSize + len >= 0xFFFFFF)
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
void PackageBufferMySQLDebugBuffer<T>::flush()
{
    if (flushed) {
        throw std::runtime_error("horsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::flush: Already flushed\n");
    }
    flushed = true;
    std::size_t   currentSize = sendBuffer.size();

    writePackageHeader(currentSize);
    writeStream(&sendBuffer[0], currentSize);
    sendBuffer.clear();
}

template<typename T>
void PackageBufferMySQLDebugBuffer<T>::reset()
{
    if (!flushed) {
        throw std::runtime_error("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::reset: reset() before message was sent with flush");
    }
    std::size_t readDataAvailable = readCurrentPacketSize - readCurrentPacketPosition;
    if (readDataAvailable == 0 && hasMore) {
        nextPacket();
        readDataAvailable = readCurrentPacketSize - readCurrentPacketPosition;
    }
    if (readDataAvailable != 0) {
        throw std::runtime_error("ThorsAnvil::MySQL::PackageBufferMySQLDebugBuffer<T>::reset: reset() before message was read");
    }

    flushed     = false;
    hasMore     = true; // Will allow us to start reading the next packet
}

template<typename T>
void PackageBufferMySQLDebugBuffer<T>::writePackageHeader(std::size_t size)
{
    ++currentPacketSequenceID;
    stream.write(reinterpret_cast<char const*>(&size), 3);
    stream.write(reinterpret_cast<char const*>(&currentPacketSequenceID), 1);
}

template<typename T>
void PackageBufferMySQLDebugBuffer<T>::writeStream(char const* buffer, std::size_t len)
{
    stream.write(buffer, len);
}

    }
}
 
