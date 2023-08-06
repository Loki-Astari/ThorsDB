#ifndef THORSANVIL_DB_COMMON_STREAM_SIMPLE_H
#define THORSANVIL_DB_COMMON_STREAM_SIMPLE_H

#include "StreamInterface.h"
#include "ThorsSocket/SSLUtil.h"

// C++
#include <string>
#include <functional>
#include <utility>
#include <memory>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef __WINNT__
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <sys/resource.h>
#endif

static std::size_t constexpr ErrorResult = static_cast<std::size_t>(-1);

#ifdef __WINNT__
inline ssize_t readMYSQLWrapper(int fd, void* buf, size_t count)
{
    ssize_t r = ::recv(fd, reinterpret_cast<char*>(buf), count, 0);
    if (r == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAENOTSOCK)
        {
            return ::_read(fd, reinterpret_cast<char*>(buf), count);
        }
        return -1;
    }
    return r;
}
inline ssize_t writeMYSQLWrapper(int fd, void const* buf, size_t count)
{
    ssize_t w = ::send(fd, reinterpret_cast<char const*>(buf), count, 0);
    if (w == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAENOTSOCK)
        {
            return ::_write(fd, reinterpret_cast<char const*>(buf), count);
        }
        return -1;
    }
    return w;
}
inline int nonBlockingMySQLWrapper(int fd)
{
    u_long mode = 1;
    int e = ::ioctlsocket(fd, FIONBIO, &mode);
    if ((e == 0) || (WSAGetLastError() == WSAENOTSOCK))
    {
        // If not an error
        // Or the error was because it was not a socket (that we ignore).
        return 0;
    }
    return -1;
}
#else
inline ssize_t readMYSQLWrapper(int fd, void* buf, size_t count)        {return ::read(fd, buf, count);}
inline ssize_t writeMYSQLWrapper(int fd, void const* buf, size_t count) {return ::write(fd, buf, count);}
inline int     nonBlockingMySQLWrapper(int fd)                          {return ::fcntl(fd, F_SETFL, O_NONBLOCK);}
#endif

namespace ThorsAnvil::DB::Common
{

class StreamSimple: public StreamInterface
{
    int                     socket;
    std::unique_ptr<ThorsIO::SSLctx> ctx;
    std::unique_ptr<ThorsIO::SSLObj> ssl;

    std::function<void()>  readYield;
    std::function<void()>  writeYield;
    public:
         StreamSimple(std::string const& host, int port, bool nonBlocking = false);
         StreamSimple(int socket);
        ~StreamSimple();
        void close();

        virtual void        read(char* buffer, std::size_t len)         override;
        virtual void        write(char const* buffer, std::size_t len)  override;
        virtual void        startNewConversation(bool)                  override {}
        virtual void        flush()                                     override {}
        virtual void        reset()                                     override {}
        virtual void        drop()                                      override {}
        virtual bool        isEmpty()                                   override {return true;}
        virtual std::string readRemainingData()                         override {return "";}

        int getSocketId() const {return socket;}
        void setYield(std::function<void()>&& yr, std::function<void()>&& yw)
        {
            readYield = std::move(yr);
            writeYield= std::move(yw);
        }

        virtual void        establishSSLConnection()                    override;
    private:
        void        readFD(char* buffer, std::size_t len);
        void        writeFD(char const* buffer, std::size_t len);
        void        readSSL(char* buffer, std::size_t len);
        void        writeSSL(char const* buffer, std::size_t len);
};

}

#endif
