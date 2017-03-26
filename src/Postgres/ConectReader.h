#ifndef THORS_ANVIL_POSTG_PACKAGE_READER_H
#define THORS_ANVIL_POSTG_PACKAGE_READER_H

#include "PackageBuffer.h"
#include <vector>
#include <string>

namespace ThorsAnvil
{
    namespace Postgres
    {

class ConectReader
{
    PackageBuffer&   stream;

    public:
        ConectReader(PackageBuffer& stream)
            : stream(stream)
        {}

        char            getMessage()        {return stream.getMessage();}
        bool            isEmpty() const     {return stream.isEmpty();}
        void            drop()              {stream.drop();}
        void            reset()             {stream.reset();}

        std::int16_t    readInt16()         {return readInt<std::int16_t>();}
        std::int32_t    readInt32()         {return readInt<std::int32_t>();}
        std::int64_t    readInt64()         {return readInt<std::int64_t>();}

        std::vector<std::int16_t>   readInt16Vector(int len)    {return readIntVector<std::int16_t>(len);}
        std::vector<std::int32_t>   readInt32Vector(int len)    {return readIntVector<std::int32_t>(len);}
        std::vector<std::int64_t>   readInt64Vector(int len)    {return readIntVector<std::int64_t>(len);}
        std::vector<char>           readByte(int len)           {return readIntVector<char>(len);}

        std::string     readStringToEnd()   {return stream.readRemainingData();}
        std::string     readString();
    private:
        char            readChar()          {return readInt<char>();}
        template<typename T>
        T readInt();
        template<typename T>
        std::vector<T> readIntVector(int len);
};


    }
}
#ifndef COVERAGE_Postgres
#include "ConectReader.tpp"
#endif

#endif
