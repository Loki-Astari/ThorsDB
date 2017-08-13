#include "MySQLStream.h"
#include "ThorSQL/SQLUtil.h"
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> // needed for memset() / bcopy()
#include <stdio.h>  // needed for strerror()
#include <fcntl.h>

using namespace ThorsAnvil::MySQL;

MySQLStream::MySQLStream(int socket)
    : socket(socket)
    , readYield([](){})
    , writeYield([](){})
{}
MySQLStream::MySQLStream(std::string const& host, int port, bool nonBlocking)
    : readYield([](){})
    , writeYield([](){})
{
    port    = port ? port : 3306;

    sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family    = AF_INET;
    serv_addr.sin_port      = htons(port);

    hostent*    serv  = ::gethostbyname(host.c_str());
    if (serv == NULL)
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::MySQLStream::MySQLStream: ",
                         "::gethostbyname() Failed: ", strerror(errno)
              ));
    }
    bcopy((char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);

    if ((socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw std::runtime_error(
                errorMsg("ThrosAnvil::MySQL::MySQLStream::MySQLStream: ",
                         "::socket() Failed: ", strerror(errno)
              ));
    }
    if (nonBlocking)
    {
        if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
        {
            throw std::domain_error(errorMsg("ThorsAnvil::MySQL::MySQLStream::MySQLStream: ",
                                                      ": fcntl: ", strerror(errno)
              ));
        }
    }

    using SockAddr = struct sockaddr;
    if (::connect(socket, reinterpret_cast<SockAddr*>(&serv_addr), sizeof(serv_addr)) < 0)
    {
        ::close(socket);
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::MySQLStream::MySQLStream: ",
                         "::connect() Failed: ", strerror(errno)
              ));
    }
}
MySQLStream::~MySQLStream()
{
    ::close(socket);
}


void MySQLStream::read(char* buffer, std::size_t len)
{
    std::size_t     readSoFar    = 0;
    while (readSoFar != len)
    {
        std::size_t read = ::read(socket, buffer + readSoFar, len - readSoFar);
        if ((read == ErrorResult) && (errno == EINTR))
        {
            /* Recoverable error. Try again. */
            continue;
        }
        else if ((read == ErrorResult) && (errno == EAGAIN || errno == ETIMEDOUT || errno == EWOULDBLOCK))
        {
            readYield();
            continue;
        }
        else if (read == 0)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::MySQLStream::read: "
                             "::read() Failed: ",
                             "Tried to read ", len, "bytes but only found ", readSoFar, " before EOF"
                  ));
        }
        else if (read == ErrorResult)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::MySQLStream::read: ",
                             "::read() Failed: ",
                             "errno=", errno, " Message=", strerror(errno)
                  ));
        }

        readSoFar += read;
    }
}
void MySQLStream::write(char const* buffer, std::size_t len)
{
    std::size_t     writenSoFar    = 0;
    while (writenSoFar != len)
    {
        std::size_t writen = ::write(socket, buffer + writenSoFar, len - writenSoFar);
        if ((writen == ErrorResult) && (errno == EINTR))
        {
            /* Recoverable error. Try again. */
            continue;
        }
        else if ((writen == ErrorResult) && (errno == EAGAIN || errno == ETIMEDOUT || errno == EWOULDBLOCK))
        {
            writeYield();
            continue;
        }
        else if (writen == 0)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::MySQLStream::write: ",
                             "::write() Failed: ",
                             "Tried to write ", len, "bytes but only found ", writenSoFar, " before EOF"
                  ));
        }
        else if (writen == ErrorResult)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::MySQL::MySQLStream::write: ",
                             "::write() Failed: ",
                             "errno=", errno, " Message=", strerror(errno)
                  ));
        }

        writenSoFar += writen;
    }
}
