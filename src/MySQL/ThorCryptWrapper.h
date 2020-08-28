#ifndef THORS_ANVIL_MYSQL_DETAILS_THOR_CRYPTO_WRAPPER_H
#define THORS_ANVIL_MYSQL_DETAILS_THOR_CRYPTO_WRAPPER_H

#ifdef  __APPLE__
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#define THOR_SHA1(data, len, dst)       CC_SHA1(data, len, dst)
#define THOR_SHA256(data, len, dst)     CC_SHA256(data, len, dst)
#else
#include <openssl/sha.h>
#define THOR_SHA1(data, len, dst)       SHA1(data, len, dst)
#define THOR_SHA256(data, len, dst)     SHA256(data, len, dst)
#endif

#include <string>

namespace ThorsAnvil::DB::MySQL
{

typedef unsigned char           ThorSHA1DigestStore[SHA_DIGEST_LENGTH];
typedef unsigned char           ThorSHA256DigestStore[SHA256_DIGEST_LENGTH];

inline void thorSHA1(ThorSHA1DigestStore& dest, ThorSHA1DigestStore& src)
{
    THOR_SHA1(src, SHA_DIGEST_LENGTH, dest);
}
inline void thorSHA1(ThorSHA1DigestStore& dest, std::string const& src)
{
    THOR_SHA1(reinterpret_cast<const unsigned char*>(&src[0]), src.length(), dest);
}
inline void thorSHA256(ThorSHA256DigestStore& dest, ThorSHA256DigestStore& src)
{
    THOR_SHA256(src, SHA256_DIGEST_LENGTH, dest);
}
inline void thorSHA256(ThorSHA256DigestStore& dest, std::string const& src)
{
    THOR_SHA256(reinterpret_cast<const unsigned char*>(&src[0]), src.length(), dest);
}

}

#endif
