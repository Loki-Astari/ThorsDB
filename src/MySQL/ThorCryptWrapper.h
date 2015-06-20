#ifndef THORSANVIL_MYSQL_DETAILS_THOR_CRYPTO_WRAPPER_H
#define THORSANVIL_MYSQL_DETAILS_THOR_CRYPTO_WRAPPER_H

#if  1

#include <openssl/sha.h>

namespace ThorsAnvil
{
    namespace MySQL
    {

typedef unsigned char           ThorSHADigestStore[SHA_DIGEST_LENGTH];
inline void thorSHA1(ThorSHADigestStore& dest, ThorSHADigestStore& src)
{
    SHA1(src, SHA_DIGEST_LENGTH, dest);
}
inline void thorSHA1(ThorSHADigestStore& dest, std::string const& src)
{
    SHA1(reinterpret_cast<const unsigned char*>(&src[0]), src.length(), dest);
}

    }
}

#else
#error  "SSL CRYPTO Library not defined"
#endif

#endif

