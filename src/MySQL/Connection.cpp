
#include "Connection.h"
#include "Statement.h"
#include "ThorSQL/Connection.h"
#include "PackageStream.h"
#include "PackageBuffer.h"
#include "ConectReader.h"
#include "RespPackageHandShake.h"
#include "RequPackageHandShakeResp.h"
#include "RespPackage.h"
#include <sstream>

using namespace ThorsAnvil::MySQL;

class DefaultMySQLConnection: public ThorsAnvil::SQL::ConnectionProxy
{
    private:
        MySQLStream                                 stream;
        PackageBufferMySQLDebugBuffer<MySQLStream>  buffer;
        ConectReader                                reader;
        ConectWriter                                writer;
        Connection                                  connection;
    public:
        DefaultMySQLConnection(std::string const& host, int port,
                               std::string const& username,
                               std::string const& password,
                               std::string const& database,
                               ThorsAnvil::SQL::Options const& options)
            : stream(host, port)
            , buffer(stream, true)
            , reader(buffer)
            , writer(buffer)
            , connection(username, password, database, options, reader, writer)
        {}
        virtual std::unique_ptr<ThorsAnvil::SQL::StatementProxy> createStatementProxy(std::string const& statement, ThorsAnvil::SQL::StatementType type) override
        {
            std::unique_ptr<ThorsAnvil::SQL::StatementProxy>  result;
            if (type == ThorsAnvil::SQL::OneTime) {
                result.reset(new Statement(statement));
            }
#if 0
            else if (type == Prepare) {
                result.reset(new PrepareStatement(statement));
            }
#endif
            else {
                throw std::runtime_error("Unknown Type for MySQL");
            }
            return result;
        }
};

ThorsAnvil::SQL::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlConnection("mysql");

Connection::Connection(
                    std::string const& username,
                    std::string const& password,
                    std::string const& database,
                    std::map<std::string, std::string> const& options,
                    ConectReader& pr,
                    ConectWriter& pw)
    : packageReader(pr)
    , packageWriter(pw)
{
    std::unique_ptr<Detail::RespPackageHandShake>    handshake = recvMessage<Detail::RespPackageHandShake>(OK, ConectReader::HandshakeOK);
    packageReader.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());
    packageWriter.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());

    Detail::RequPackageHandShakeResponse    handshakeresp(username, password, options, database, *handshake);
    std::unique_ptr<RespPackage>            ok = sendMessage<RespPackage>(handshakeresp, Reset, OK, ConectReader::HandshakeOK);
}

Connection::~Connection()
{
    // Shut Down
}

#ifdef COVERAGE_TEST
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "Connection.tpp"

#endif

