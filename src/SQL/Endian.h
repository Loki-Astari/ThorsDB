#ifndef THORSANVIL_UTIL_ENDIAN_H
#define THORSANVIL_UTIL_ENDIAN_H

#include <type_traits>
#include <arpa/inet.h>

#ifdef htonll
#undef htonll
#undef ntohll
#endif

#ifndef htonll
#if THOR_ENDIAN_BIG
inline std::uint64_t htonll(std::uint64_t value)
{
    return value;
}
inline std::uint64_t ntohll(std::uint64_t value)
{
    return value;
}
#else
inline std::uint64_t htonll(std::uint64_t value)
{
    uint64_t  result;
    uint32_t* inputValue    = reinterpret_cast<uint32_t*>(&value);
    uint32_t* outputValue   = reinterpret_cast<uint32_t*>(&result);

    outputValue[0] = htonl(inputValue[1]);
    outputValue[1] = htonl(inputValue[0]);
    return result;
}
inline std::uint64_t ntohll(std::uint64_t value)
{
    uint64_t  result;
    uint32_t* inputValue    = reinterpret_cast<uint32_t*>(&value);
    uint32_t* outputValue   = reinterpret_cast<uint32_t*>(&result);

    outputValue[0] = htonl(inputValue[1]);
    outputValue[1] = htonl(inputValue[0]);
    return result;
}
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
