#include "ThorMySQL.h"
#include "RequPackageSSLRequest.h"

using namespace ThorsAnvil::DB::MySQL;

RequPackageSSLRequest::RequPackageSSLRequest(long capabilities)
    : RequPackage("RequPackageSSLRequest", "SSL Request")
    , capabilities(capabilities)
{}

void RequPackageSSLRequest::build(ConectWriter& writer) const
{
    // https://dev.mysql.com/doc/internals/en/connection-phase-packets.html#packet-Protocol::SSLRequest
    // Turn off flags not supported by the server
    writer.writeFixedLengthInteger<4>(capabilities);

    long maxPacketSize   = 0x01000000;
    writer.writeFixedLengthInteger<4>(maxPacketSize);

    unsigned char charset = 0x21;
    writer.writeFixedLengthInteger<1>(charset);

    std::string reserved(23, '\0');
    writer.writeFixedLengthString(reserved, 23);
}

std::ostream& RequPackageSSLRequest::print(std::ostream& s) const
{
    return s << "RequPackageSSLRequest: "
             << "capabilities(" << capabilities << ") ";
}
