#ifndef THORS_ANVIL_MYSQL_AUTHENTICATION_H
#define THORS_ANVIL_MYSQL_AUTHENTICATION_H

#include "ThorSQL/SQLUtil.h"
#include "ThorCryptWrapper.h"
#include <string>
#include <ostream>

namespace ThorsAnvil
{
    namespace MySQL
    {

using ThorsAnvil::SQL::Options;

inline std::string authentication(std::string const& plugin, std::string const& pluginData,
               std::string const& /*username*/,
               std::string const& password,
               Options const&     /*options*/,
               std::string const& /*database*/)
{
    if (plugin == "mysql_old_password")
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                         "mysql_old_password: not supported yet. Feel free to add handler."
              ));
    }
    else if (plugin == "mysql_clear_password")
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                         "mysql_clear_password: not supported yet. Feel free to add handler."
              ));
    }
    else if (plugin == "authentication_windows_client")
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                         "authentication_windows_client: not supported yet. Feel free to add handler."
              ));
    }
    else if (plugin == "sha256_password")
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                         "sha256_password: not supported yet. Feel free to add handler."
              ));
    }
    else if (plugin == "caching_sha2_password")
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                         "caching_sha2_password: not supported yet. Feel free to add handler."
              ));
    }
    else if (plugin == "mysql_native_password")
    {
        // Requires CLIENT_SECURE_CONNECTION
        // SHA1( password ) XOR SHA1( "20-bytes random data from server" <concat> SHA1( SHA1( password ) ) )
        ThorSHADigestStore  stage1;
        thorSHA1(stage1, password);


        ThorSHADigestStore  stage2;
        thorSHA1(stage2, stage1);

        std::string extendedAuth = pluginData + std::string(stage2, stage2 + SHA_DIGEST_LENGTH);
        ThorSHADigestStore  extendedHash;
        thorSHA1(extendedHash, extendedAuth);

        for (int loop=0;loop < SHA_DIGEST_LENGTH;++loop)
        {
            extendedHash[loop] = extendedHash[loop] ^ stage1[loop];
        }
        return std::string(extendedHash, extendedHash + SHA_DIGEST_LENGTH);
    }
    else
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                         "UNKNOWN authentication method(", plugin, "): not supported"
              ));
    }
}

    }
}

#endif
