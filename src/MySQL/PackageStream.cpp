
#include "PackageStream.h"
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <cerrno>

using namespace ThorsAnvil::MySQL;

MySQLStream::MySQLStream(std::string const& host, int port)
{
    port    = port ? port : 3306;

    sockaddr_in serv_addr; 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family    = AF_INET;
    serv_addr.sin_port      = htons(port); 

    hostent*    serv  = gethostbyname(host.c_str());
    if (serv == NULL) {
        throw std::runtime_error(std::string("MySQL::connect: ::gethostbyname() Failed: ") + strerror(errno));
    }
    bcopy((char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);

    if ((socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error(std::string("MySQL::Connection::connect: ::socket() Failed: ") + strerror(errno));
    } 

    if (::connect(socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        ::close(socket);
        throw std::runtime_error(std::string("MySQL::Connection::connect: ::connect() Failed: ") + strerror(errno));
    } 
}
MySQLStream::~MySQLStream()
{
    ::close(socket);
}


void MySQLStream::read(char* buffer, std::size_t len)
{
    std::size_t     readSoFar    = 0;
    while(readSoFar != len)
    {
        std::size_t read = ::read(socket, buffer + readSoFar, len - readSoFar);
        if ((read == ErrorResult) && (errno == EAGAIN || errno == EINTR)) {
            /* Recoverable error. Try again. */
            continue;
        }
        else if (read == 0) {
            std::stringstream msg;
            msg << "ThorsAnvil::MySQLStream::read: Read fail. Tried to read " << len << "bytes but only found " << readSoFar << " before EOF";
            throw std::runtime_error(msg.str());
        }
        else if (read == ErrorResult)
        {
            std::stringstream msg;
            msg << "ThorsAnvil::MySQLStream::read: Read fail. errno=" << errno << " Message=" << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        readSoFar += read;
    }
}
void MySQLStream::write(char const* buffer, std::size_t len)
{
    std::size_t     writenSoFar    = 0;
    while(writenSoFar != len)
    {
        std::size_t writen = ::write(socket, buffer + writenSoFar, len - writenSoFar);
        if ((writen == ErrorResult) && (errno == EAGAIN || errno == EINTR)) {
            /* Recoverable error. Try again. */
            continue;
        }
        else if (writen == 0) {
            std::stringstream msg;
            msg << "ThorsAnvil::MySQLStream::write: Write fail. Tried to write " << len << "bytes but only found " << writenSoFar << " before EOF";
            throw std::runtime_error(msg.str());
        }
        else if (writen == ErrorResult)
        {
            std::stringstream msg;
            msg << "ThorsAnvil::MySQLStream::write: Write fail. errno=" << errno << " Message=" << strerror(errno);
            throw std::runtime_error(msg.str());
        }

        writenSoFar += writen;
    }
}

