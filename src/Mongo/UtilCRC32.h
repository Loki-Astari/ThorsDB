#ifndef THROSANVIL_DB_MONGO_UTIL_CRC_H
#define THROSANVIL_DB_MONGO_UTIL_CRC_H

#include <cstdint>

namespace ThorsAnvil::DB::Mongo
{

extern std::uint32_t crcTable[256];

inline
std::uint32_t crc32CheckSum(char const* buffer, std::size_t count)
{
    static constexpr std::uint32_t mask         = 0xFFFFFFFFul;
    static constexpr std::uint32_t tableMask    = 0x000000FFul;

    std::uint32_t checkSum = 0xFFFFFFFFul;
    unsigned char const*    data = reinterpret_cast<unsigned char const*>(buffer);

    for (std::size_t loop = 0; loop < count; ++loop)
    {
        checkSum = (checkSum >> 8) ^ crcTable[ (checkSum ^ static_cast<std::uint32_t>(*data)) & tableMask ];
        ++data;
    }

    return (checkSum ^ mask);
}

}

#endif
