#include "Authentication.h"
#include "Connection.h"
#include "ConectWriter.h"
#include "ConectReader.h"
#include "RespPackage.h"
#include "RequPackageHandShakeResp.h"
#include "RequPackageAuthSwitchResp.h"
#include "RespPackageAuthSwitchRequest.h"

#include "ThorCryptWrapper.h"
#include <ostream>

using namespace ThorsAnvil::MySQL;

Authetication::Authetication(Connection& connection, Options const& options)
    : connection(connection)
    , options(options)
{}

Authetication::~Authetication()
{}

std::unique_ptr<RespPackage> Authetication::sendHandShakeResponse(std::string const& username,
                                                                  std::string const& password,
                                                                  std::string const& database,
                                                                  std::string const& pluginData,
                                                                  long capabilities,
                                                                  unsigned long charset)
{
    std::string authResponse  = getAuthenticationString(username, password, database, pluginData);
    RequPackageHandShakeResponse    handshakeresp(username, password, options, database,
                                                  capabilities,
                                                  getPluginName(),
                                                  authResponse);

    connection.initFromHandshake(handshakeresp.getCapabilities(), charset);

    return connection.sendHandshakeMessage<RespPackage>(handshakeresp,
        {
         // Connection::conectToServer() assumes only responses are OK/Error/RespPackageAuthSwitchRequest
         // If this changes to allow other responses then look at Connection::conectToServer() where we
         // handle RespPackageAuthSwitchRequest
         {0xFE, [](int firstByte, ConectReader& reader)
                {return new RespPackageAuthSwitchRequest(firstByte, reader);}
         }
        });
}

std::unique_ptr<RespPackage> Authetication::sendSwitchResponse(std::string const& username,
                                                               std::string const& password,
                                                               std::string const& database,
                                                               std::string const& pluginData)
{
    std::string authResponse  = getAuthenticationString(username, password, database, pluginData);

    RequPackageAuthSwitchResponse   switchResp(username, password, options, database, authResponse);
    return connection.sendHandshakeMessage<RespPackage>(switchResp, {});
}

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
        virtual std::string getPluginName() const override
        {
            return "mysql_native_password";
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
        else if (authPluginNameUsed == "caching_sha2_password")
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::HandshakeResponsePackage::HandshakeResponsePackage: ",
                             "caching_sha2_password: not supported yet. Feel free to add handler."
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
