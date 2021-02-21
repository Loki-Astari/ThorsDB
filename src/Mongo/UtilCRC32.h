#ifndef THROSANVIL_DB_MONGO_UTIL_CRC_H
#define THROSANVIL_DB_MONGO_UTIL_CRC_H

#include <cstdint>

namespace ThorsAnvil::DB::Mongo
{

class CRC32
{
    protected:
        static const std::uint32_t table[256];
};

class CRC32C
{
    protected:
        static const std::uint32_t table[256];
};

template<typename Type>
class CheckSum: public Type
{
    static constexpr std::uint32_t mask         = 0xFFFFFFFFul;
    static constexpr std::uint32_t tableMask    = 0x000000FFul;

    std::uint32_t   checkSum;
    public:
        CheckSum()                      {reset();}
        void          reset()           {checkSum = 0xFFFFFFFFul;}
        std::uint32_t checksum() const  {return (checkSum ^ mask);}
        void append(char const* buffer, std::size_t count)
        {
            unsigned char const*    data = reinterpret_cast<unsigned char const*>(buffer);

            for (std::size_t loop = 0; loop < count; ++loop)
            {
                checkSum = (checkSum >> 8) ^ Type::table[(checkSum ^ static_cast<std::uint32_t>(*data)) & tableMask];
                ++data;
            }
        }
};

using CRC32_Checksum    = CheckSum<CRC32>;
using CRC32C_Checksum   = CheckSum<CRC32C>;

}

#endif
