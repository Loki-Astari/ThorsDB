#include "MongoBuffer.h"
#include "snappy.h"
#include <iostream>


using namespace ThorsAnvil::DB::Mongo;

MongoBuffer::MongoBuffer(ThorsIO::DataSocket& stream,
            ThorsIO::Notifier noAvailableData, ThorsIO::Notifier flushing,
            std::vector<char>&& bufData,
            char const* currentStart, char const* currentEnd)
    : ThorsIO::SocketStreamBufferBase(stream, std::move(noAvailableData), std::move(flushing), in, out)
    , in(std::move(bufData))
    , out(4000)
{
    char* newStart = const_cast<char*>(currentStart);
    char* newEnd   = const_cast<char*>(currentEnd);
    if (newStart == nullptr || newEnd == nullptr)
    {
        newStart = &in[0];
        newEnd   = &in[0];
    }

    setg(&in[0], newStart, newEnd);
    setp(&out[0], &out[out.size() - 1]);
}

MongoBuffer::MongoBuffer(MongoBuffer&& move) noexcept
    : ThorsIO::SocketStreamBufferBase(std::move(move), in, out)
    , in(std::move(move.in))
    , out(std::move(move.out))
{
    move.setg(nullptr, nullptr, nullptr);
    move.setp(nullptr, nullptr);
}

MongoBufferSnappy::MongoBufferSnappy(MongoBuffer&& move)
    : MongoBuffer(std::move(move))
{}

std::streamsize MongoBufferSnappy::xsputn(char const* s,std::streamsize count)
{
    return MongoBuffer::xsputn(s, count);
}

int MongoBufferSnappy::sync()
{
    std::string output;
    snappy::Compress(pbase() + 16, (pptr() - pbase()) - 16, &output);

    std::uint32_t   compressedSize      = output.size();
    std::uint32_t   compressedSizeLE    = boost::endian::native_to_little(compressedSize + 25);
    std::uint32_t   opCode              = boost::endian::native_to_little(2012);
    std::uint32_t   originalSize        = boost::endian::little_to_native(*reinterpret_cast<std::uint32_t*>(pbase()));
    std::uint32_t   originalBodySize    = boost::endian::native_to_little(originalSize - 16);
    std::uint8_t    snappy              = 1;

    writeToStream(reinterpret_cast<char const*>(&compressedSizeLE), sizeof(compressedSizeLE));
    writeToStream(pbase() + 4,  8);
    writeToStream(reinterpret_cast<char const*>(&opCode), sizeof(compressedSizeLE));
    writeToStream(pbase() + 12, 4);
    writeToStream(reinterpret_cast<char const*>(&originalBodySize), sizeof(originalBodySize));
    writeToStream(reinterpret_cast<char const*>(&snappy), sizeof(snappy));
    writeToStream(&output[0], output.size());

    setp(&out[0], &out[out.size() - 1]);

    return 0;
}

void MongoBufferSnappy::decompress(std::uint32_t compressedSize, std::uint32_t uncompressedSize)
{
    std::uint32_t   currentSize = egptr() - gptr();
    std::uint32_t   extraNeeded = compressedSize - currentSize;
    resizeInputBuffer(compressedSize + 25); // Add the header back

    // Make sure we have all the data from the stream in the buffer.
    readFromStream(gptr(), extraNeeded, true);
    setg(eback(), gptr(), egptr() + extraNeeded);

    // Have a buffer to put the result in.
    uncompressedBuffer.clear();
    uncompressedBuffer.reserve(uncompressedSize);

    // Uncompress into our new buffer and make that the place to read from
    snappy::Uncompress(gptr(), compressedSize, &uncompressedBuffer);
    setg(&uncompressedBuffer[0], &uncompressedBuffer[0], &uncompressedBuffer[uncompressedBuffer.size()]);

    // Note: an `underflow()` will reset the buffer back to the `in` buffer of the
    // underlying type.
}
