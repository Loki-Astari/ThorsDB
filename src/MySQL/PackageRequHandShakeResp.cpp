
#include "PackageRequHandShakeResp.h"
#include "PackageRespHandShake.h"
#include "ThorMySQL.h"
#include "ThorCryptWrapper.h"

using namespace ThorsAnvil::MySQL::Detail;

PackageRequHandShakeResponse::PackageRequHandShakeResponse(std::string const& username,
                                                   std::string const& password,
                                                   std::string const& database,
                                                   long capabilities,
                                                   PackageRespHandShake const& handshake)
    : PackageRequ("PackageRequHandShakeResponse")
    , username(username)
    , database(database)
    , capabilities(capabilities)
{
    if (handshake.getAuthPluginName() == "mysql_old_password")
    {   throw std::runtime_error("HandshakeResponsePackage::HandshakeResponsePackage: mysql_old_password: not supported");
    }
    else if (handshake.getAuthPluginName() == "mysql_clear_password")
    {   throw std::runtime_error("HandshakeResponsePackage::HandshakeResponsePackage: mysql_clear_password: not supported");
    }
    else if (handshake.getAuthPluginName() == "authentication_windows_client")
    {   throw std::runtime_error("HandshakeResponsePackage::HandshakeResponsePackage: authentication_windows_client: not supported");
    }
    else if (handshake.getAuthPluginName() == "sha256_password")
    {   throw std::runtime_error("HandshakeResponsePackage::HandshakeResponsePackage: sha256_password: not supported");
    }
    else if (handshake.getAuthPluginName() == "mysql_native_password")
    {
        // Requires CLIENT_SECURE_CONNECTION
        // SHA1( password ) XOR SHA1( "20-bytes random data from server" <concat> SHA1( SHA1( password ) ) )
        ThorSHADigestStore  stage1;
        thorSHA1(stage1, password);


        ThorSHADigestStore  stage2;
        thorSHA1(stage2, stage1);

        std::string extendedAuth = handshake.getAuthPluginData() + std::string(stage2, stage2 + SHA_DIGEST_LENGTH);
        ThorSHADigestStore  extendedHash;
        thorSHA1(extendedHash, extendedAuth);

        for(int loop=0;loop < SHA_DIGEST_LENGTH;++loop)
        {
            extendedHash[loop] = extendedHash[loop] ^ stage1[loop];
        }
        authResponse    = std::string(extendedHash, extendedHash + SHA_DIGEST_LENGTH);
        authPluginName  = handshake.getAuthPluginName();
    }
    else
    {   throw std::runtime_error("HandshakeResponsePackage::HandshakeResponsePackage: UNKNOWN authentication method(" + handshake.getAuthPluginName() + "): not supported");
    }
}

void PackageRequHandShakeResponse::send(PackageConWriter& writer) const
{
    // These capabilities mirror the `mysql` tool.
    // We will leave this for now but it may change
    // as the understanding of the system updates.
    long cap  = CLIENT_SET_CLIENT | CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA | CLIENT_CONNECT_ATTRS
                 | CLIENT_PLUGIN_AUTH | CLIENT_PS_MULTI_RESULTS | CLIENT_MULTI_RESULTS | CLIENT_MULTI_STATEMENTS
                 | CLIENT_SECURE_CONNECTION | CLIENT_TRANSACTIONS
                 | CLIENT_INTERACTIVE | CLIENT_PROTOCOL_41
                 | CLIENT_LOCAL_FILES
                 | CLIENT_CONNECT_WITH_DB | CLIENT_LONG_FLAG | CLIENT_LONG_PASSWORD;
    writer.writeFixedLengthInteger<4>(cap);

    long maxPacketSize   = 0x01000000;
    writer.writeFixedLengthInteger<4>(maxPacketSize);

    unsigned char charset = 0x21;
    writer.writeFixedLengthInteger<1>(charset);

    std::string reserved(23, '\0');
    writer.writeFixedLengthString(reserved, 23);

    writer.writeNullTerminatedString(username);

    if (capabilities & CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA)
    {
        writer.writeLengthEncodedInteger(authResponse.size());
        writer.writeVariableLengthString(authResponse);
    }
    else if (capabilities & CLIENT_SECURE_CONNECTION)
    {
        char val = authResponse.size();
        writer.writeFixedLengthInteger<1>(val);
        writer.writeVariableLengthString(authResponse);
    }
    else
    {
        writer.writeNullTerminatedString(authResponse);
    }

    if (capabilities & CLIENT_CONNECT_WITH_DB)
    {
        writer.writeNullTerminatedString(database);
    }

    if (capabilities & CLIENT_PLUGIN_AUTH)
    {
        writer.writeNullTerminatedString(authPluginName);
    }

    // TODO Add Key Values
    if (capabilities & CLIENT_CONNECT_ATTRS)
    {
        writer.writeLengthEncodedInteger(0);
        /*
          Integer Length encoded  length of all key-values        (if connection.getCapabilities() & CLIENT_CONNECT_ATTRS)
          String Len Encoded      Key                             (if connection.getCapabilities() & CLIENT_CONNECT_ATTRS)
          String Len Encoded      Value                           (if connection.getCapabilities() & CLIENT_CONNECT_ATTRS)
        */
    }
}

std::ostream& PackageRequHandShakeResponse::print(std::ostream& s) const
{
    return s;
}

