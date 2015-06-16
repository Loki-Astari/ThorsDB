
#include "Connection.h"
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>

using namespace ThorsAnvil::MySQL;

ThorsAnvil::SQL::ConnectionCreatorRegister<ThorsAnvil::MySQL::Connection>    mysqlConnection("mysql");

PackageReader   Connection::defaultPackageReader;

Connection::Connection(
                    std::string const& host, int port,
                    std::string const& /*username*/,
                    std::string const& /*password*/,
                    std::map<std::string, std::string> const& /*options*/,
                    PackageReader& packageReader)
    : socket(connect(host, port))
    , packageReader(packageReader)
{
    // Send connection handshake
}

Connection::~Connection()
{
    // Shut Down
    packageReader.test();
}

int Connection::connect(std::string const& host, int port)
{
    int socket;
    if ((socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {   throw std::runtime_error(std::string("MySQL::Connection::connect: ::socket() Failed: ") + strerror(errno));
    } 

    port    = port ? port : 3306;

    sockaddr_in serv_addr; 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family    = AF_INET;
    serv_addr.sin_port      = htons(port); 

    hostent*    serv  = gethostbyname(host.c_str());
    if (serv == NULL)
    {   throw std::runtime_error(std::string("MySQL::connect: ::gethostbyname() Failed: ") + strerror(errno));
    }
    bcopy((char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);

    if (::connect(socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {   throw std::runtime_error(std::string("MySQL::Connection::connect: ::connect() Failed: ") + strerror(errno));
    } 
    return socket;
}

