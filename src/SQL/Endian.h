#ifndef THORS_ANVIL_DB_UTIL_ENDIAN_H
#define THORS_ANVIL_DB_UTIL_ENDIAN_H

#include <arpa/inet.h>

#ifndef htonll

namespace ThorsAnvil::DB::Util
{

enum class EndianTypes
{
    Error, Little, Big
};

enum EndinatTestValue : uint32_t
{
    LittleEndian   = 0x00000001,
    BigEndian      = 0x01000000,
};

#pragma vera-pushoff
constexpr EndianTypes getEndianType()
#pragma vera-pop
{
    return ((0xFFFFFFFF & 1) == EndinatTestValue::LittleEndian)
            ? EndianTypes::Little
            : ((0xFFFFFFFF & 1) == EndinatTestValue::BigEndian)
                ? EndianTypes::Big
                : EndianTypes::Error;
}

template<EndianTypes>
struct Endian;

template<>
struct Endian<EndianTypes::Little>
{
    static std::uint64_t ntohll_Impl(std::uint64_t value)
    {
        uint64_t  result;
        uint32_t* inputValue    = reinterpret_cast<uint32_t*>(&value);
        uint32_t* outputValue   = reinterpret_cast<uint32_t*>(&result);

        outputValue[0] = htonl(inputValue[1]);
        outputValue[1] = htonl(inputValue[0]);
        return result;
    }
    static std::uint64_t htonll_Impl(std::uint64_t value)
    {
        return ntohll_Impl(value);
    }
};

template<>
struct Endian<EndianTypes::Big>
{
    static std::uint64_t ntohll_Impl(std::uint64_t value)
    {
        return value;
    }
    static std::uint64_t htonll_Impl(std::uint64_t value)
    {
        return value;
    }
};

}

inline std::uint64_t ntohll(std::uint64_t value)
{
    return ThorsAnvil::DB::Util::Endian<ThorsAnvil::DB::Util::getEndianType()>::ntohll_Impl(value);
}
inline std::uint64_t htonll(std::uint64_t value)
{
    return ThorsAnvil::DB::Util::Endian<ThorsAnvil::DB::Util::getEndianType()>::htonll_Impl(value);
}
#endif

namespace ThorsAnvil::DB::Util
{

inline unsigned char convertToHost(unsigned char value)
{
    return value;
}
inline std::uint16_t convertToHost(std::uint16_t value)
{
    return ntohs(value);
}
inline std::uint32_t convertToHost(std::uint32_t value)
{
    return ntohl(value);
}
inline std::uint64_t convertToHost(std::uint64_t value)
{
    return ntohll(value);
}
inline unsigned char convertToNet(unsigned char value)
{
    return value;
}
inline std::uint16_t convertToNet(std::uint16_t value)
{
    return htons(value);
}
inline std::uint32_t convertToNet(std::uint32_t value)
{
    return htonl(value);
}
inline std::uint64_t convertToNet(std::uint64_t value)
{
    return htonll(value);
}

}

#endif
