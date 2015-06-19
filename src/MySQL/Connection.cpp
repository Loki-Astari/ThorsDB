
#include "Connection.h"
#include "ThorSQL/Connection.h"
#include "PackageStream.h"
#include "PackageBuffer.h"
#include "PackageReader.h"

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
    packageReader.getNextPackage();
    // Send connection handshake
}

Connection::~Connection()
{
    // Shut Down
}

