#ifndef THORS_ANVIL_DB_POSTGRES_CONECT_WRITER_H
#define THORS_ANVIL_DB_POSTGRES_CONECT_WRITER_H

#include "PackageBuffer.h"

#include <vector>
#include <string>

namespace ThorsAnvil::DB::Postgres
{

class ConectWriter
{
    protected:
    PackageBuffer&   stream;

    public:

        ConectWriter(PackageBuffer& stream)
            : stream(stream)
        {}
        virtual ~ConectWriter() {}

        void        sendMessage(char m) {return stream.sendMessage(m);}
        void        flush()             {stream.flush();}
        void        reset()             {stream.reset();}

        void writeChar(char value)          {return writeInt<char>(value);}
        void writeInt16(std::int16_t value) {return writeInt<std::int16_t>(value);}
        void writeInt32(std::int32_t value) {return writeInt<std::int32_t>(value);}
        void writeInt64(std::int64_t value) {return writeInt<std::int64_t>(value);}

        void writeInt16Vector(std::vector<std::int16_t> const& data)    {writeIntVector<std::int16_t>(data);}
        void writeInt32Vector(std::vector<std::int32_t> const& data)    {writeIntVector<std::int32_t>(data);}
        void writeInt64Vector(std::vector<std::int64_t> const& data)    {writeIntVector<std::int64_t>(data);}
        void writeByte(std::vector<char> const& data)                   {writeIntVector<char>(data);}

        void writeString(std::string const& data);

    private:
        template<typename T>
        void writeInt(T value);
        template<typename T>
        void writeIntVector(std::vector<T> const& data);
};

}

#include "ConectWriter.tpp"

#endif
