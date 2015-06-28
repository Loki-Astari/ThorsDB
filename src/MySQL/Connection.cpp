
#include "Connection.h"
#include "Statement.h"
#include "ThorSQL/Connection.h"
#include "PackageStream.h"
#include "PackageBuffer.h"
#include "ConectReader.h"
#include "RespPackageHandShake.h"
#include "RequPackageHandShakeResp.h"
#include "PrepareStatement.h"
#include "RespPackage.h"
#include "RespPackageOK.h"
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
        virtual std::unique_ptr<ThorsAnvil::SQL::StatementProxy> createStatementProxy(std::string const& statement, ThorsAnvil::SQL::StatementType /*type*/) override
        {
            std::unique_ptr<ThorsAnvil::SQL::StatementProxy>  result;
            result.reset(new PrepareStatement(connection, statement));
#if 0
            if (type == ThorsAnvil::SQL::OneTime) {
                result.reset(new Statement(statement));
            }
            else if (type == Prepare) {
                result.reset(new PrepareStatement(statement));
            }
            else {
                throw std::runtime_error("Unknown Type for MySQL");
            }
#endif
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
    std::unique_ptr<Detail::RespPackageHandShake>    handshake = recvMessage<Detail::RespPackageHandShake>(OK, [](ConectReader& reader){return std::unique_ptr<RespPackage>(new Detail::RespPackageHandShake(reader));});
    packageReader.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());
    packageWriter.initFromHandshake(handshake->getCapabilities(), handshake->getCharset());

    Detail::RequPackageHandShakeResponse    handshakeresp(username, password, options, database, *handshake);
    std::unique_ptr<RespPackage>            ok = sendMessage<RespPackage>(handshakeresp, None, OK, [](ConectReader& reader){return std::unique_ptr<RespPackage>(new Detail::RespPackageOK(reader));});
}

Connection::~Connection()
{
    // Shut Down
}

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "Connection.tpp"

template std::unique_ptr<ThorsAnvil::MySQL::Detail::RespPackagePrepare> ThorsAnvil::MySQL::Connection::sendMessage<ThorsAnvil::MySQL::Detail::RespPackagePrepare, ThorsAnvil::MySQL::Detail::RequPackagePrepare>(ThorsAnvil::MySQL::Detail::RequPackagePrepare const&, ThorsAnvil::MySQL::Connection::PacketContinuation, ThorsAnvil::MySQL::Connection::PacketCompletion, std::function<std::unique_ptr<ThorsAnvil::MySQL::RespPackage>(ThorsAnvil::MySQL::ConectReader&)>);

#endif

