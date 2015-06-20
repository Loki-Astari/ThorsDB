
#include "Connection.h"
#include "ThorSQL/Connection.h"
#include "PackageStream.h"
#include "PackageBuffer.h"
#include "PackageReader.h"
#include "PackageRespHandShake.h"
#include "PackageResp.h"
#include <sstream>

using namespace ThorsAnvil::MySQL;

class DefaultMySQLConnection: public ThorsAnvil::SQL::ConnectionProxy
{
    private:
        MySQLStream                                 stream;
        PackageBufferMySQLDebugBuffer<MySQLStream>  buffer;
        PackageReader                               reader;
    public:
        DefaultMySQLConnection(std::string const& host, int port,
                               std::string const& /*username*/,
                               std::string const& /*password*/,
                               ThorsAnvil::SQL::Options const& /*options*/)
            : stream(host, port)
            , buffer(stream)
            , reader(buffer)
        {}
};

ThorsAnvil::SQL::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlConnection("mysql");

Connection::Connection(
                    std::string const& /*username*/,
                    std::string const& /*password*/,
                    std::map<std::string, std::string> const& /*options*/,
                    PackageReader& pr)
    : packageReader(pr)
{
    std::unique_ptr<PackageResp>    initConnect = packageReader.getNextPackage(PackageReader::HandshakeOK);
    if (initConnect->isError())
    {
        std::stringstream message;
        message << *initConnect;
        throw std::runtime_error(message.str());
    }

    Detail::PackageRespHandShake&   handshake = dynamic_cast<Detail::PackageRespHandShake&>(*initConnect);
    packageReader.initFromHandshake(handshake.getCapabilities(), handshake.getCharset());

    //HandshakeResponsePackage        handshakeresp(*this, handshage.getCharset(), username, password, database, handshake));
    //std::unique_ptr<PackageRead>    ok(handshakeresp.getResponsePackage<PackageRead>(PackageWrite::Reset, PackageWrite::OK, PackageRead::HandshakeOK));
}

Connection::~Connection()
{
    // Shut Down
}

