#include "StreamSimple.h"
#include "ThorSQL/SQLUtil.h"
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <netdb.h>
#include <unistd.h>
#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif
#include <errno.h>
#include <string.h> // needed for memset() / bcopy()
#include <stdio.h>  // needed for strerror()

using namespace ThorsAnvil::SQL;

StreamSimple::StreamSimple(int socket)
    : socket(socket)
{}
StreamSimple::StreamSimple(std::string const& host, int port)
{
    sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family    = AF_INET;
    serv_addr.sin_port      = htons(port);

    hostent*    serv  = ::gethostbyname(host.c_str());
    if (serv == NULL)
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::SQL::StreamSimple::StreamSimple: ",
                         "::gethostbyname() Failed: ", strerror(errno)
              ));
    }
    bcopy((char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);

    if ((socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw std::runtime_error(
                errorMsg("ThrosAnvil::SQL::StreamSimple::StreamSimple: ",
                         "::socket() Failed: ", strerror(errno)
              ));
    }

    using SockAddr = struct sockaddr;
    if (::connect(socket, reinterpret_cast<SockAddr*>(&serv_addr), sizeof(serv_addr)) < 0)
    {
        ::close(socket);
        throw std::runtime_error(
                errorMsg("ThorsAnvil::SQL::StreamSimple::StreamSimple: ",
                         "::connect() Failed: ", strerror(errno)
              ));
    }
}
StreamSimple::~StreamSimple()
{
    ::close(socket);
}


void StreamSimple::read(char* buffer, std::size_t len)
{
    std::size_t     readSoFar    = 0;
    while (readSoFar != len)
    {
        std::size_t read = ::read(socket, buffer + readSoFar, len - readSoFar);
        if ((read == ErrorResult) && (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK))
        {
            /* Recoverable error. Try again. */
            continue;
        }
        else if (read == 0)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::SQL::StreamSimple::read: "
                             "::read() Failed: ",
                             "Tried to read ", len, "bytes but only found ", readSoFar, " before EOF"
                  ));
        }
        else if (read == ErrorResult)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::SQL::StreamSimple::read: ",
                             "::read() Failed: ",
                             "errno=", errno, " Message=", strerror(errno)
                  ));
        }

        readSoFar += read;
    }
}
void StreamSimple::write(char const* buffer, std::size_t len)
{
    std::size_t     writenSoFar    = 0;
    while (writenSoFar != len)
    {
        std::size_t writen = ::write(socket, buffer + writenSoFar, len - writenSoFar);
        if ((writen == ErrorResult) && (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK))
        {
            /* Recoverable error. Try again. */
            continue;
        }
        else if (writen == ErrorResult)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::SQL::StreamSimple::write: ",
                             "::write() Failed: ",
                             "errno=", errno, " Message=", strerror(errno)
                  ));
        }

        writenSoFar += writen;
    }
}
