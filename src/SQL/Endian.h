#ifndef THORSANVIL_UTIL_ENDIAN_H
#define THORSANVIL_UTIL_ENDIAN_H

#include <arpa/inet.h>

#ifndef htonll

enum ThorEndianTestValues
{
    O32_LITTLE_ENDIAN   = 0x03020100ul,
    O32_BIG_ENDIAN      = 0x00010203ul,
    O32_PDP_ENDIAN      = 0x01000302ul
};

static constexpr union { unsigned char bytes[4]; uint32_t value; } thorHostOrderCheck = { { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)

#if O32_HOST_ORDER == O32_LITTLE_ENDIAN
inline std::uint64_t htonll(std::uint64_t value)
{
    uint64_t  result;
    uint32_t* inputValue    = reinterpret_cast<uint32_t*>(&value);
    uint32_t* outputValue   = reinterpret_cast<uint32_t*>(&result);

    outputValue[0] = htonl(inputValue[1]);
    outputValue[1] = htonl(inputValue[0]);
    return result;
}
inline std::uint64_t ntohll(std::uint64_t value) {return htonll(value);}
#elif O32_HOST_ORDER == O32_BIG_ENDIAN
inline std::uint64_t htonll(std::uint64_t value)
{
    return value;
}
inline std::uint64_t ntohll(std::uint64_t value)
{
    return value;
}
#elif O32_HOST_ORDER == O32_PDP_ENDIAN
#error "Please write a htonll() for machines of PDP Endian"
#endif
#endif

namespace ThorsAnvil
{
    namespace Util
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
    return ntohs(value);
}
inline std::uint32_t convertToNet(std::uint32_t value)
{
    return ntohl(value);
}
inline std::uint64_t convertToNet(std::uint64_t value)
{
    return ntohll(value);
}

    }
}

#endif
