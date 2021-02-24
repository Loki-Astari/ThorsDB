#ifndef THORS_ANVIL_DB_MONGO_MONGO_BUFFER_H
#define THORS_ANVIL_DB_MONGO_MONGO_BUFFER_H

#include "Util.h"
#include "ThorsSocket/SocketStream.h"
#include "snappy.h"
#include <boost/endian/conversion.hpp>
#include <string>
#include <vector>

namespace ThorsAnvil::DB::Mongo
{

class MongoBuffer: public ThorsIO::SocketStreamBufferBase
{
    protected:
        std::vector<char>       in;
        std::vector<char>       out;

    public:
        MongoBuffer(ThorsIO::DataSocket& stream,
                    ThorsIO::Notifier noAvailableData, ThorsIO::Notifier flushing,
                    std::vector<char>&& bufData = std::vector<char>(4000),
                    char const* currentStart = nullptr, char const* currentEnd = nullptr);
        MongoBuffer(MongoBuffer&& move) noexcept;

        template<typename Func>
        void apply(Func& func)
        {
            func.append(std::string_view(pbase(), pptr() - pbase()));
        }
};

class MongoBufferSnappy: public MongoBuffer
{
    std::string   uncompressedBuffer;
    char*         inputBufferSave[3];
    public:
        MongoBufferSnappy(MongoBuffer&& move);
        virtual int sync() override;
        virtual std::streamsize xsputn(char const* s,std::streamsize count) override;

        virtual int underflow() override;
        void decompress(std::uint32_t compressedSize, std::uint32_t uncompressedSize);
};

}

#endif
