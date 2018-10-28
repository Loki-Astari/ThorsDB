#include "Authentication.h"
#include "Connection.h"
#include "ConectWriter.h"
#include "ConectReader.h"
#include "RespPackage.h"
#include "RequPackageHandShakeResp.h"
#include "RequPackageAuthSwitchResp.h"
#include "RespPackageAuthSwitchRequest.h"
#include "RespPackageAuthMoreData.h"
#include "RespPackageERR.h"

#include "ThorCryptWrapper.h"
#include <ostream>

using namespace ThorsAnvil::MySQL;

Authetication::Authetication(Connection& connection, Options const& options)
    : connection(connection)
    , options(options)
{}

Authetication::~Authetication()
{}

class AutheticationMySQLNativePassword: public Authetication
{
    public:
        AutheticationMySQLNativePassword(Connection& connection, Options const& options)
            : Authetication(connection, options)
        {}

        virtual std::string getAuthenticationString(
                       std::string const& /*username*/,
                       std::string const& password,
                       std::string const& /*database*/,
                       std::string const& pluginData) override
        {
            // Requires CLIENT_SECURE_CONNECTION
            // SHA1( password ) XOR SHA1( "20-bytes random data from server" <concat> SHA1( SHA1( password ) ) )
            ThorSHA1DigestStore  stage1;
            thorSHA1(stage1, password);


            ThorSHA1DigestStore  stage2;
            thorSHA1(stage2, stage1);

            std::string extendedAuth = pluginData + std::string(stage2, stage2 + SHA_DIGEST_LENGTH);
            ThorSHA1DigestStore  extendedHash;
            thorSHA1(extendedHash, extendedAuth);

            for (int loop=0;loop < SHA_DIGEST_LENGTH;++loop)
            {
                extendedHash[loop] = extendedHash[loop] ^ stage1[loop];
            }
            return std::string(extendedHash, extendedHash + SHA_DIGEST_LENGTH);
        }
        virtual std::string getPluginName() const override
        {
            return "mysql_native_password";
        }
};
class AutheticationCachingSHA2Password: public Authetication
{
    public:
        AutheticationCachingSHA2Password(Connection& connection, Options const& options)
            : Authetication(connection, options)
        {}

        virtual std::string getAuthenticationString(
                       std::string const& /*username*/,
                       std::string const& password,
                       std::string const& /*database*/,
                       std::string const& pluginData) override
        {
            // Empty Password is special.
            if (password == "")
            {
                return "";
            }

            // Fast Authentication start
            return scramble(password, pluginData);

        }
        std::string scramble(std::string const& password, std::string const& pluginData)
        {
            // Scramble - XOR(SHA256(password), SHA256(SHA256(SHA256(password)), Nonce))
            ThorSHA256DigestStore  stage1;
            thorSHA256(stage1, password);

            ThorSHA256DigestStore  stage2;
            thorSHA256(stage2, stage1);

            std::string extendedAuth = std::string(stage2, stage2 + SHA256_DIGEST_LENGTH) + pluginData;

            ThorSHA256DigestStore  stage3;
            thorSHA256(stage3, extendedAuth);

            for (int loop=0;loop < SHA256_DIGEST_LENGTH;++loop)
            {
                stage3[loop] = stage3[loop] ^ stage1[loop];
            }
            return std::string(stage3, stage3 + SHA256_DIGEST_LENGTH);

        }
        virtual std::string getPluginName() const override
        {
            return "caching_sha2_password";
        }
        virtual std::unique_ptr<RespPackage> customAuthenticate(std::unique_ptr<RespPackageAuthMoreData> msg,
                                                                std::string const& /*username*/,
                                                                std::string const& /*password*/,
                                                                std::string const& /*database*/,
                                                                std::string const& /*pluginData*/) override
        {
            //static char constexpr request_public_key            = '\2';
            static char constexpr fast_auth_success             = '\3';
            //static char constexpr perform_full_authentication   = '\4';

            std::string const& msgData = msg->getPluginMoreData();
            if (msgData.size() == 1 && msgData[0] == fast_auth_success)
            {
                // Should be the OK message.
                return connection.recvMessage();
            }
            return nullptr;
        }
};

namespace ThorsAnvil
{
    namespace MySQL
    {

std::unique_ptr<Authetication> getAuthenticatonMethod(Connection& connection, std::string const& authPluginName, Options const& options)
{
    std::unique_ptr<Authetication> result;

    auto auth = options.find("default-auth");
    std::string const& authPluginNameUsed = (auth != options.end()) ? auth->second : authPluginName;
    //authResponse = authentication(authPluginNameUsed, handshake.getAuthPluginData(), username, password, options, database);

    if (authPluginNameUsed == "mysql_native_password")
    {
        return std::unique_ptr<Authetication>(new AutheticationMySQLNativePassword(connection, options));
    }
    else if (authPluginNameUsed == "caching_sha2_password")
    {
        return std::unique_ptr<Authetication>(new AutheticationCachingSHA2Password(connection, options));
    }
    else
    {
        using ThorsAnvil::errorMsg;
        // All other methods are unsupported.
        // But it is nice to have specific error messages to show that we know they exist.
        if (authPluginNameUsed == "mysql_old_password")
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                             "mysql_old_password: not supported yet. Feel free to add handler."
                  ));
        }
        else if (authPluginNameUsed == "mysql_clear_password")
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                             "mysql_clear_password: not supported yet. Feel free to add handler."
                  ));
        }
        else if (authPluginNameUsed == "authentication_windows_client")
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                             "authentication_windows_client: not supported yet. Feel free to add handler."
                  ));
        }
        else if (authPluginNameUsed == "sha256_password")
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                             "sha256_password: not supported yet. Feel free to add handler."
                  ));
        }
        else
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                             "UNKNOWN authentication method(", authPluginNameUsed, "): not supported"
                  ));
        }
    }
}

    }
}
