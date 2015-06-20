
#include "Connection.h"
#include "ThorSQL/Connection.h"
#include "PackageStream.h"
#include "PackageBuffer.h"
#include "PackageConReader.h"
#include "PackageRespHandShake.h"
#include "PackageResp.h"
#include <sstream>

using namespace ThorsAnvil::MySQL;

class DefaultMySQLConnection: public ThorsAnvil::SQL::ConnectionProxy
{
    private:
        MySQLStream                                 stream;
        PackageBufferMySQLDebugBuffer<MySQLStream>  buffer;
        PackageConReader                            reader;
        PackageConWriter                            writer;
        Connection                                  connection;
    public:
        DefaultMySQLConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               ThorsAnvil::SQL::Options const& options)
            : stream(host, port)
            , buffer(stream)
            , reader(buffer)
            , writer(buffer)
            , connection(username, password, options, reader, writer)
        {}
};

ThorsAnvil::SQL::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlConnection("mysql");

Connection::Connection(
                    std::string const& /*username*/,
                    std::string const& /*password*/,
                    std::map<std::string, std::string> const& /*options*/,
                    PackageConReader& pr,
                    PackageConWriter& pw)
    : packageReader(pr)
    , packageWriter(pw)
{
    std::unique_ptr<Detail::PackageRespHandShake>    handshake = recvMessage<Detail::PackageRespHandShake>(PackageConReader::HandshakeOK, PackageConReader::OK);
    packageReader.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());
    packageWriter.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());

    //HandshakeResponsePackage        handshakeresp(*this, handshage.getCharset(), username, password, database, handshake));
    //std::unique_ptr<PackageRead>    ok(handshakeresp.getResponsePackage<PackageRead>(PackageWrite::Reset, PackageWrite::OK, PackageRead::HandshakeOK));
}

Connection::~Connection()
{
    // Shut Down
}

