#include "ThorSQL/SQLUtil.h"
#include <iomanip>

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename T>
PackageBuffer<T>::PackageBuffer(T& stream, bool flushed)
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
inline void PackageBuffer<T>::read(char* buffer, std::size_t len)
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
    while (len != retrieved);
}

template<typename T>
bool PackageBuffer<T>::isEmpty()
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
std::string PackageBuffer<T>::readRemainingData()
{
    std::string dst;
    do
    {
        long retrieved  = dst.size();
        long remaining  = readCurrentPacketSize - readCurrentPacketPosition;
        dst.resize(retrieved + remaining);
        read(&dst[retrieved], remaining);
        if (hasMore)
        {
            nextPacket();
            continue;
        }
        break;
    }
    while (true);

    return dst;
}

template<typename T>
void PackageBuffer<T>::nextPacket()
{
    if (!hasMore)
    {
        throw std::domain_error(
                errorMsg("ThorsAnvil::MySQL::PackageBuffer::nextPacket: ",
                         "No more data expected from server"
              ));
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
    {
        throw std::domain_error(
                errorMsg("ThorsAnvil::MySQL::PackageBuffer::nextPacket: ",
                         "currentPacketSequenceID(", currentPacketSequenceID, ")",
                         " != actual sequence on input stream(",  actualSequenceID, ")"
              ));
    }

    hasMore = readCurrentPacketSize == 0xFFFFFF;
}

template<typename T>
void PackageBuffer<T>::startNewConversation()
{
    currentPacketSequenceID = -1;
}

template<typename T>
void PackageBuffer<T>::write(char const* buffer, std::size_t len)
{
    if (flushed)
    {
        throw std::domain_error(
                bugReport("ThorsAnvil::MySQL::PackageBuffer::write: ",
                         "Writting to a flushed buffer"
              ));
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
void PackageBuffer<T>::flush()
{
    if (flushed)
    {
        throw std::domain_error(
                bugReport("ThorsAnvil::MySQL::PackageBuffer<T>::flush: ",
                          "Already flushed"
              ));
    }
    flushed = true;
    std::size_t   currentSize = sendBuffer.size();

    writePackageHeader(currentSize);
    writeStream(&sendBuffer[0], currentSize);
    sendBuffer.clear();
}

template<typename T>
void PackageBuffer<T>::drop()
{
    std::size_t dataLeft;
    std::vector<char>   drop;
    do
    {
        std::size_t readDataAvailable = readCurrentPacketSize - readCurrentPacketPosition;
        drop.resize(readDataAvailable);
        read(&drop[0], readDataAvailable);

        dataLeft    = 0;
        if (hasMore)
        {
            nextPacket();
            dataLeft = readCurrentPacketSize - readCurrentPacketPosition;
        }
    }
    while (dataLeft != 0);
}

template<typename T>
void PackageBuffer<T>::reset()
{
    std::size_t readDataAvailable = readCurrentPacketSize - readCurrentPacketPosition;
    if (readDataAvailable == 0 && hasMore)
    {
        nextPacket();
        readDataAvailable = readCurrentPacketSize - readCurrentPacketPosition;
    }
    if (readDataAvailable != 0)
    {
        std::stringstream  extraData;
        for (std::size_t loop=0; loop < readDataAvailable; ++loop)
        {
            char x;
            read(&x, 1);
            extraData << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(x) << "(" << x << ") ";
        }
        throw std::domain_error(
                bugReport("ThorsAnvil::MySQL::PackageBuffer<T>::reset: ",
                          "reset() before message was read:",
                          extraData.str()
              ));
    }

    flushed     = false;
    hasMore     = true; // Will allow us to start reading the next packet
}

template<typename T>
void PackageBuffer<T>::writePackageHeader(std::size_t size)
{
    ++currentPacketSequenceID;
    stream.write(reinterpret_cast<char const*>(&size), 3);
    stream.write(reinterpret_cast<char const*>(&currentPacketSequenceID), 1);
}

template<typename T>
void PackageBuffer<T>::writeStream(char const* buffer, std::size_t len)
{
    stream.write(buffer, len);
}

    }
}
