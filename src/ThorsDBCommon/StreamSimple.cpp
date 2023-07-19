#include "StreamSimple.h"
#include "ThorsIOUtil/Utility.h"
#include "ThorsLogging/ThorsLogging.h"
#include <stdexcept>
#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

// C++
#include <iostream>
#include <sstream>

// C
#include <errno.h>
#include <fcntl.h>
#include <iostream>

using namespace ThorsAnvil::DB::Common;
using ThorsAnvil::Utility::buildErrorMessage;
using ThorsAnvil::Utility::systemErrorMessage;

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
        ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                         "StreamSimple",
                         "::gethostbyname() Failed: ", systemErrorMessage());
    }
    char* src = reinterpret_cast<char*>(serv->h_addr);
    char* dst = reinterpret_cast<char*>(&serv_addr.sin_addr.s_addr);
    std::copy(src, src + serv->h_length, dst);

    if ((socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ThorsLogAndThrow("ThrosAnvil::SQL::StreamSimple",
                         "StreamSimple",
                         "::socket() Failed: ", systemErrorMessage());
    }
    if (nonBlocking)
    {
        if (::nonBlockingMySQLWrapper(socket) == -1)
        {
            ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::StreamSimple",
                                     "StreamSimple",
                                     "::fcntl() ", systemErrorMessage());
        }
    }
    using SockAddr = struct sockaddr;
    if (::connect(socket, reinterpret_cast<SockAddr*>(&serv_addr), sizeof(serv_addr)) < 0)
    {
        // Not an error if it is in progress
        if (errno != EINPROGRESS)
        {
            ::close(socket);
            ThorsLogAndThrow("ThorsAnvil::DB::MySQL::StreamSimple",
                             "StreamSimple",
                             "::connect() Failed: ", systemErrorMessage());
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
            ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                             "readFD",
                             "::read() Failed: ",
                             "Tried to read ", len, "bytes but only found ", readSoFar, " before EOF");
        }
        else if (read == ErrorResult)
        {
            ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                             "readFD",
                             "::read() Failed: ", " Message=", systemErrorMessage());
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
            ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                             "writeFD",
                             "::write() Failed: ",
                             "Tried to write ", len, "bytes but only found ", writenSoFar, " before EOF");
        }
        else if (writen == ErrorResult)
        {
            ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                             "writeFD",
                             "::write() Failed: ", " Message=", systemErrorMessage());
        }

        writenSoFar += writen;
    }
}
void StreamSimple::readSSL(char* buffer, std::size_t len)
{
    using ThorsAnvil::ThorsIO::IOInfo;

    std::size_t     readSoFar    = 0;
    while (readSoFar != len)
    {
        IOInfo read = ssl->read(socket, buffer + readSoFar, len - readSoFar);
        if (read.first < 0)
        {
            int errorCode = ssl->nativeErrorCode(read.first);
            if (errorCode == SSL_ERROR_WANT_READ)
            {
                readYield();
                continue;
            }
            else
            {
                ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                                 "readSSL",
                                 "::SSL_read() Failed: ",
                                 "errno=", errorCode, " Message=", ThorsIO::SSLUtil::errorMessage());
            }
        }
        else if (read.first == 0)
        {
            ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                             "readSSL",
                             "SSL_read() Failed: ",
                             "Tried to read ", len, "bytes but only found ", readSoFar, " before EOF");
        }

        readSoFar += read.first;
    }
}
void StreamSimple::writeSSL(char const* buffer, std::size_t len)
{
    using ThorsAnvil::ThorsIO::IOInfo;

    std::size_t     writenSoFar    = 0;
    while (writenSoFar != len)
    {
        IOInfo writen = ssl->write(socket, buffer + writenSoFar, len - writenSoFar);
        if (writen.first < 0)
        {
            int errorCode = ssl->nativeErrorCode(writen.first);
            if (errorCode == SSL_ERROR_WANT_WRITE)
            {
                writeYield();
                continue;
            }
            else
            {
                ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                                 "writeSSL",
                                 "::SSL_write() Failed: ",
                                 "errno=", errorCode, " Message=", ThorsIO::SSLUtil::errorMessage());
            }
        }
        else if (writen.first == 0)
        {
            ThorsLogAndThrow("ThorsAnvil::DB::SQL::StreamSimple",
                             "writeSSL",
                             "::SSL_write() Failed: ",
                             "Tried to write ", len, "bytes but only found ", writenSoFar, " before EOF");
        }

        writenSoFar += writen.first;
    }
}

void StreamSimple::establishSSLConnection()
{
    ThorsIO::SSLMethod   method(ThorsIO::SSLMethodType::Client);
    ctx     = std::make_unique<ThorsIO::SSLctx>(method);
    ssl     = std::make_unique<ThorsIO::SSLObj>(*ctx, socket);
    ssl->doConnect();
}
