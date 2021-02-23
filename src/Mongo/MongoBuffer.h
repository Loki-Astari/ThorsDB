#ifndef THORS_ANVIL_DB_MONGO_MONGO_BUFFER_H
#define THORS_ANVIL_DB_MONGO_MONGO_BUFFER_H

#include "ThorsSocket/SocketStream.h"
#include <string>
#include <vector>

namespace ThorsAnvil::DB::Mongo
{

class MongoBuffer: public ThorsIO::SocketStreamBufferBase
{
    private:
        std::vector<char>       in;
        std::vector<char>       out;

    public:
        MongoBuffer(ThorsIO::DataSocket& stream,
                    ThorsIO::Notifier noAvailableData, ThorsIO::Notifier flushing,
                    std::vector<char>&& bufData = std::vector<char>(4000),
                    char const* currentStart = nullptr, char const* currentEnd = nullptr)
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
        MongoBuffer(MongoBuffer&& move) noexcept
            : ThorsIO::SocketStreamBufferBase(std::move(move), in, out)
            , in(std::move(move.in))
            , out(std::move(move.out))
        {
            move.setg(nullptr, nullptr, nullptr);
            move.setp(nullptr, nullptr);
        }

        template<typename Func>
        void apply(Func& func)
        {
            func.append(std::string_view(pbase(), pptr() - pbase()));
        }
};

}

#endif
