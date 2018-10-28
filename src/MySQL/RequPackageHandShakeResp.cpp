#include "ThorMySQL.h"
#include "Authentication.h"
#include "RequPackageHandShakeResp.h"
#include "RespPackageHandShake.h"
#include "ThorCryptWrapper.h"

using namespace ThorsAnvil::MySQL;

// These capabilities mirror the `mysql` tool.
// We will leave this for now but it may change
// as the understanding of the system updates.
constexpr unsigned long cap  =
    CLIENT_SET_CLIENT | CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA | CLIENT_CONNECT_ATTRS
  | CLIENT_PLUGIN_AUTH | CLIENT_PS_MULTI_RESULTS | CLIENT_MULTI_RESULTS | CLIENT_MULTI_STATEMENTS
  | CLIENT_SECURE_CONNECTION | CLIENT_TRANSACTIONS
  | CLIENT_INTERACTIVE | CLIENT_PROTOCOL_41
  | CLIENT_LOCAL_FILES
  // | SERVER_SESSION_STATE_CHANGED
  // We don't know how to decode the Session State Information in the RespPackageOK
  // So this must currently be turned off.
  // Once you have it decoding you can enable this.
  | CLIENT_CONNECT_WITH_DB | CLIENT_LONG_FLAG | CLIENT_LONG_PASSWORD
  | CLIENT_SSL;

RequPackageHandShakeResponse::RequPackageHandShakeResponse(std::string const& username,
                                                           std::string const& /*password*/,
                                                           Options const&     options,
                                                           std::string const& database,
                                                           long               capabilities,
                                                           std::string const& authPluginName,
                                                           std::string const& authResponse)
    : RequPackage("RequPackageHandShakeResponse", "HandShake-Response")
    , username(username)
    , authResponse(authResponse)
    , options(options)
    , database(database)
    , authPluginName(authPluginName)
    , capabilities(capabilities & cap)
{}

void RequPackageHandShakeResponse::build(ConectWriter& writer) const
{
    // https://dev.mysql.com/doc/internals/en/connection-phase-packets.html#packet-Protocol::HandshakeResponse41
    // Turn off flags not supported by the server
    writer.writeFixedLengthInteger<4>(capabilities);

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
        writer.writeFixedLengthString(authResponse, authResponse.size());
    }
    else if (capabilities & CLIENT_SECURE_CONNECTION)
    {
        char val = authResponse.size();
        writer.writeFixedLengthInteger<1>(val);
        writer.writeFixedLengthString(authResponse, authResponse.size());
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
    // For now empty the options
    if (capabilities & CLIENT_CONNECT_ATTRS)
    {
        std::size_t size = 0;
        for (auto const& loop: options)
        {
            size    += loop.first.size() + loop.second.size();
        }
        writer.writeLengthEncodedInteger(size);
        for (auto const& loop: options)
        {
            writer.writeLengthEncodedString(loop.first);
            writer.writeLengthEncodedString(loop.second);
        }
    }
}

std::ostream& RequPackageHandShakeResponse::print(std::ostream& s) const
{
    std::stringstream authRespDecoded;
    for (char x: authResponse)
    {   authRespDecoded << "0x" << std::hex << static_cast<unsigned int>(static_cast<unsigned char>(x)) << " ";
    }
    std::stringstream keyValDecoded;
    for (auto const& val: options)
    {   keyValDecoded << "KV(" << val.first << " => " << val.second << ") ";
    }

    return s << "HandshakeResponsePackage: "
             << "username(" << username << ") "
             << "authResponse(" << authRespDecoded.str() << ") "
             << "options(" << keyValDecoded.str() << ") "
             << "database(" << database << ") "
             << "authPluginName(" << authPluginName << ") "
             << "capabilities(" << capabilities << ") ";
}
