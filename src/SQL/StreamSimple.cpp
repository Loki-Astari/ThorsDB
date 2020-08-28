#include "StreamSimple.h"
#include "SQLUtil.h"
#include <stdexcept>
#include <sys/socket.h>
#include <netdb.h>
#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

// C++
#include <iostream>
#include <sstream>

// C
#include <errno.h>
#include <string.h> // needed for memset() / bcopy()
#include <stdio.h>  // needed for strerror()
#include <fcntl.h>
#include <iostream>

using namespace ThorsAnvil::DB::SQL;

StreamSimple::StreamSimple(int socket)
    : socket(socket)
    , readYield([](){})
    , writeYield([](){})
{}
StreamSimple::StreamSimple(std::string const& host, int port, bool nonBlocking)
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
                errorMsg("ThorsAnvil::DB::SQL::StreamSimple::StreamSimple: ",
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
    if (nonBlocking)
    {
        if (::fcntlMYSQLWrapper(socket, F_SETFL, O_NONBLOCK) == -1)
        {
            throw std::domain_error(errorMsg("ThorsAnvil::DB::MySQL::StreamSimple::StreamSimple: ",
                                                      ": fcntl: ", strerror(errno)
              ));
        }
    }
    using SockAddr = struct sockaddr;
    if (::connect(socket, reinterpret_cast<SockAddr*>(&serv_addr), sizeof(serv_addr)) < 0)
    {
        // Not an error if it is in progress
        if (errno != EINPROGRESS)
        {
            ::close(socket);
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::DB::MySQL::StreamSimple::StreamSimple: ",
                             "::connect() Failed: ", strerror(errno)
                  ));
        }
    }
}
StreamSimple::~StreamSimple()
{
    if (socket != -1)
    {
        close();
    }
}
void StreamSimple::close()
{
    ssl.reset(nullptr);
    ctx.reset(nullptr);
    ::close(socket);
    socket = -1;
}


void StreamSimple::read(char* buffer, std::size_t len)
{
    if (ssl)
    {
        readSSL(buffer, len);
    }
    else
    {
        readFD(buffer, len);
    }
}
void StreamSimple::write(char const* buffer, std::size_t len)
{
    if (ssl)
    {
        writeSSL(buffer, len);
    }
    else
    {
        writeFD(buffer, len);
    }
}
void StreamSimple::readFD(char* buffer, std::size_t len)
{
    std::size_t     readSoFar    = 0;
    while (readSoFar != len)
    {
        std::size_t read = ::readMYSQLWrapper(socket, buffer + readSoFar, len - readSoFar);
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
                    errorMsg("ThorsAnvil::DB::SQL::StreamSimple::readFD: "
                             "::read() Failed: ",
                             "Tried to read ", len, "bytes but only found ", readSoFar, " before EOF"
                  ));
        }
        else if (read == ErrorResult)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::DB::SQL::StreamSimple::readFD: ",
                             "::read() Failed: ",
                             "errno=", errno, " Message=", strerror(errno)
                  ));
        }

        readSoFar += read;
    }
}
void StreamSimple::writeFD(char const* buffer, std::size_t len)
{
    std::size_t     writenSoFar    = 0;
    while (writenSoFar != len)
    {
        std::size_t writen = ::writeMYSQLWrapper(socket, buffer + writenSoFar, len - writenSoFar);
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
                    errorMsg("ThorsAnvil::DB::SQL::StreamSimple::writeFD: ",
                             "::write() Failed: ",
                             "Tried to write ", len, "bytes but only found ", writenSoFar, " before EOF"
                  ));
        }
        else if (writen == ErrorResult)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::DB::SQL::StreamSimple::writeFD: ",
                             "::write() Failed: ",
                             "errno=", errno, " Message=", strerror(errno)
                  ));
        }

        writenSoFar += writen;
    }
}
void StreamSimple::readSSL(char* buffer, std::size_t len)
{
    std::size_t     readSoFar    = 0;
    while (readSoFar != len)
    {
        int read = ssl->read(buffer + readSoFar, len - readSoFar);
        if (read < 0)
        {
            int errorCode = ssl->errorCode(read);
            if (errorCode == SSL_ERROR_WANT_READ)
            {
                readYield();
                continue;
            }
            else
            {
                throw std::runtime_error(
                        errorMsg("ThorsAnvil::DB::SQL::StreamSimple::readSSL: ",
                                 "::SSL_read() Failed: ",
                                 "errno=", errorCode, " Message=", SSLUtil::errorMessage()
                      ));
            }
        }
        else if (read == 0)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::DB::SQL::StreamSimple::readSSL: "
                             "SSL_read() Failed: ",
                             "Tried to read ", len, "bytes but only found ", readSoFar, " before EOF"
                  ));
        }

        readSoFar += read;
    }
}
void StreamSimple::writeSSL(char const* buffer, std::size_t len)
{
    std::size_t     writenSoFar    = 0;
    while (writenSoFar != len)
    {
        int writen = ssl->write(buffer + writenSoFar, len - writenSoFar);
        if (writen < 0)
        {
            int errorCode = ssl->errorCode(writen);
            if (errorCode == SSL_ERROR_WANT_WRITE)
            {
                writeYield();
                continue;
            }
            else
            {
                throw std::runtime_error(
                        errorMsg("ThorsAnvil::DB::SQL::StreamSimple::writeSSL: ",
                                 "::SSL_write() Failed: ",
                                 "errno=", errorCode, " Message=", SSLUtil::errorMessage()
                      ));
            }
        }
        else if (writen == 0)
        {
            throw std::runtime_error(
                    errorMsg("ThorsAnvil::DB::SQL::StreamSimple::writeSSL: ",
                             "::SSL_write() Failed: ",
                             "Tried to write ", len, "bytes but only found ", writenSoFar, " before EOF"
                  ));
        }

        writenSoFar += writen;
    }
}

void StreamSimple::establishSSLConnection()
{
    SSLMethod   method(SSLMethodType::Client);
    ctx     = std::make_unique<SSLctx>(method);
    ssl     = std::make_unique<SSLObj>(*ctx, socket);
    ssl->connect();
}
