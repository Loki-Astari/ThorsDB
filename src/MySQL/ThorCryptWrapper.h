#ifndef THORSANVIL_MYSQL_DETAILS_THOR_CRYPTO_WRAPPER_H
#define THORSANVIL_MYSQL_DETAILS_THOR_CRYPTO_WRAPPER_H

#ifdef  __APPLE__
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#define THOR_SHA1(data, len, dst)   CC_SHA1(data, len, dst)
#else
#include <openssl/sha.h>
#define THOR_SHA1(data, len, dst)   SHA1(data, len, dst)
#endif

namespace ThorsAnvil
{
    namespace MySQL
    {

typedef unsigned char           ThorSHADigestStore[SHA_DIGEST_LENGTH];
inline void thorSHA1(ThorSHADigestStore& dest, ThorSHADigestStore& src)
{
    THOR_SHA1(src, SHA_DIGEST_LENGTH, dest);
}
inline void thorSHA1(ThorSHADigestStore& dest, std::string const& src)
{
    THOR_SHA1(reinterpret_cast<const unsigned char*>(&src[0]), src.length(), dest);
}

    }
}

#endif
