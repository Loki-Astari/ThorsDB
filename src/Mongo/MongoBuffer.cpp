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
    , inputBufferSave{nullptr, nullptr, nullptr}
{}

std::streamsize MongoBufferSnappy::xsputn(char const* s,std::streamsize count)
{
    return MongoBuffer::xsputn(s, count);
}

int MongoBufferSnappy::sync()
{
    // This is a compressing buffer
    // So we take the input stream we are given and compress it while sending it
    // to the underlying stream.
    // Note: It is assumed that called has already made sure that the buffer was large
    //       enough to hold the complete uncompressed message.
    //       Unfortunately the mongo compression is done in such a way that you
    //       need the whole object in memory before compression so that you can build
    //       and send the header (you need the compressed size up front).
    //
    // The format of the output is assumed to be:
    //      MsgHeader
    //          32bit           Size
    //          32bit           Id
    //          32bit           Id
    //          32bit           OpCode
    //      Msg Block
    //          <Data>          (Size - 16 bytes)
    //
    // This reformats the message to:
    //      MsgHeader
    //          32bit           Size => 16(Header) + 9(OP_COMPRESS Info) + <Compressed Data Size>
    //          32bit           Id
    //          32bit           Id
    //          32bit           OpCode: OP_COMPRESS
    //      OP_COPRESS MESSAGE
    //          32bit           Original OpCode
    //          32bit           Data Uncompressed Size
    //          8bit            Compression Type: 1=>Snappy
    //          <Compressed Data>
    //

    std::string output;
    snappy::Compress(pbase() + 16, (pptr() - pbase()) - 16, &output);

    std::uint32_t   compressedSize      = output.size();
    std::uint32_t   newMessageSize      = boost::endian::native_to_little(16 + 9 + compressedSize);
    std::uint32_t   opCode              = boost::endian::native_to_little(2012);
    std::uint32_t   originalSize        = boost::endian::little_to_native(*reinterpret_cast<std::uint32_t*>(pbase()));
    std::uint32_t   DataUncompressedSize= boost::endian::native_to_little(originalSize - 16);
    std::uint8_t    snappy              = 1;

    writeToStream(reinterpret_cast<char const*>(&newMessageSize), sizeof(newMessageSize));
    writeToStream(pbase() + 4,  8);
    writeToStream(reinterpret_cast<char const*>(&opCode), sizeof(opCode));
    writeToStream(pbase() + 12, 4);
    writeToStream(reinterpret_cast<char const*>(&DataUncompressedSize), sizeof(DataUncompressedSize));
    writeToStream(reinterpret_cast<char const*>(&snappy), sizeof(snappy));
    writeToStream(&output[0], output.size());

    setp(&out[0], &out[out.size() - 1]);

    return 0;
}

int MongoBufferSnappy::underflow()
{
    if (inputBufferSave[0] != nullptr)
    {
        // If we have saved the end of a buffer during a decompress() operation
        // restore this part of the buffer.
        setg(inputBufferSave[0], inputBufferSave[1], inputBufferSave[2]);
        inputBufferSave[0] = nullptr;
    }
    if (gptr() != egptr())
    {
        // If there is any data available return it.
        return traits::to_int_type(*gptr());
    }
    // Otherwise use the parent to fulfill the request.
    return MongoBuffer::underflow();
}

void MongoBufferSnappy::decompress(std::uint32_t compressedSize, std::uint32_t uncompressedSize)
{
    // Note:
    //  The Op_MsgHeader reads off the OP_COMPRESSED data from the stream.
    //  So the input stream should now only include the compressed data.
    //
    //  So this function reads the compressed data and uncompresses it
    //  then sets it as the input source for subsequent reads from the stream.

    std::uint32_t   currentSize = egptr() - gptr();
    std::uint32_t   extraNeeded = compressedSize - currentSize;
    resizeInputBuffer(compressedSize + 25); // Add the header back
                                            // This will prevent a move that we don't need.

    // Make sure we have all the data from the stream in the buffer.
    readFromStream(egptr(), extraNeeded, true);
    setg(eback(), gptr(), egptr() + extraNeeded);

    // Have a buffer to put the result in.
    uncompressedBuffer.clear();
    uncompressedBuffer.reserve(uncompressedSize);

    // Uncompress into our new buffer and make that the place to read from
    snappy::Uncompress(gptr(), compressedSize, &uncompressedBuffer);
    gbump(compressedSize);
    // Save the current input buffer.
    // When we have finished reading the uncompressed data we need to switch back to this.
    inputBufferSave[0] = gbase();
    inputBufferSave[1] = gtr();
    inputBufferSave[2] = egptr();

    // Set the input buffer to the uncompressed version of the data.
    setg(&uncompressedBuffer[0], &uncompressedBuffer[0], &uncompressedBuffer[uncompressedBuffer.size()]);

    // Note: an `underflow()` will reset the buffer back to the `in` buffer of the
    // underlying type.
}
