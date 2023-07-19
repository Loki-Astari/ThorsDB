#include "test/pipe.h"
#include "StreamSimple.h"
#include "ThorsSocket/SSLUtil.h"
#include "ThorsIOUtil/Utility.h"
#include "ThorsLogging/ThorsLogging.h"
#include "coverage/ThorMock.h"
#include <fstream>
#include <thread>
#include <gtest/gtest.h>

#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/time.h>

using ThorsAnvil::Utility::buildErrorMessage;
using ThorsAnvil::Utility::systemErrorMessage;
using ThorsAnvil::DB::Common::StreamSimple;

TEST(StreamSimpleTest, ReadNormal)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    char data[16];
    stream.read(data,16);

    ASSERT_EQ(std::string(data, data + 16), std::string("1234567890ABCDEF"));
}
TEST(StreamSimpleTest, ReadInterupt)
{
    SocketSetUp     setupSockets;

    MOCK_SYS(readMYSQLWrapper, [](int socket, void* buffer, std::size_t len) -> ssize_t
    {
        static bool firstTime = true;
        if (firstTime)
        {
            firstTime = false;
            errno = EINTR;
            return -1L;
        }
        return ::read(socket, reinterpret_cast<char*>(buffer), len);
    });
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    char data[16];
    stream.read(data,16);

    ASSERT_EQ(std::string(data, data + 16), std::string("1234567890ABCDEF"));
}
TEST(StreamSimpleTest, ReadGoodHost)
{
    SocketSetUp     setupSockets;

    StreamSimple stream("google.com", 80);
}
TEST(StreamSimpleTest, NonBlockingFail)
{
    SocketSetUp     setupSockets;

    MOCK_SYS(nonBlockingMySQLWrapper, [](int){return -1;});
    auto test = []()
    {
        StreamSimple stream("google.com", 80, true);
    };

    EXPECT_THROW(
        test(),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(StreamSimpleTest, NonBlockingOK)
{
    SocketSetUp     setupSockets;

    MOCK_SYS(nonBlockingMySQLWrapper, [](int){return 0;});
    StreamSimple stream("google.com", 80, true);
}
TEST(StreamSimpleTest, GetCoverageOnTheWrapper)
{
    SocketSetUp     setupSockets;

    nonBlockingMySQLWrapper(-1);
}
TEST(StreamSimpleTest, ReadGoodHostBadPort)
{
    SocketSetUp     setupSockets;

    // ::connect should fail
    ASSERT_THROW(
        StreamSimple stream("127.0.0.1", 1),
        std::runtime_error
    );
}
TEST(StreamSimpleTest, ReadGoodHostSocketFail)
{
#ifdef __WINNT__
    GTEST_SKIP() << "Windows does not support getrlimit";
#else

    SocketSetUp     setupSockets;

    int sysres;

    struct rlimit   curLimit;
    sysres = getrlimit(RLIMIT_NOFILE, &curLimit);
    ASSERT_EQ(0, sysres);

    struct rlimit   newLimit = curLimit;
    newLimit.rlim_cur = 1;
    sysres = setrlimit(RLIMIT_NOFILE, &newLimit);
    ASSERT_EQ(0, sysres);

    ASSERT_THROW(
        StreamSimple stream("127.0.0.1", 80),
        std::runtime_error
    );

    sysres = setrlimit(RLIMIT_NOFILE, &curLimit);
    ASSERT_EQ(0, sysres);
#endif
}
TEST(StreamSimpleTest, ReadFromSlowStreamToGetEAGAIN)
{
#ifdef __WINNT__
    GTEST_SKIP() << "Windows does not support non blocking pipes";
#else
    SocketSetUp     setupSockets;

    int sysres;
    int testData   = 5;
    int resultData = 0;

    int pipes[2];
    sysres = ::pipe(pipes);
    ASSERT_EQ(0, sysres);
    int flags = ::fcntl(pipes[0], F_GETFL, 0);
    ASSERT_NE(-1, flags);
    sysres = ::fcntl(pipes[0], F_SETFL, flags | O_NONBLOCK);
    ASSERT_NE(-1, sysres);

    std::thread slowStream([testData, &pipes](){
        for(int loop=0;loop < sizeof(testData); ++loop) {
            sleep(1);
            ::write(pipes[1], reinterpret_cast<char const*>(&testData)+loop, 1);
        }
    });

    StreamSimple stream(pipes[0]);
    stream.read(reinterpret_cast<char*>(&resultData), sizeof(resultData));

    ASSERT_EQ(testData, resultData);
    //::close(pipes[0]);  // closed by stream
    ::close(pipes[1]);
    slowStream.join();
#endif
}
TEST(StreamSimpleTest, ReadBadHost)
{
    SocketSetUp     setupSockets;

    ASSERT_THROW(
        StreamSimple stream("BadHostNotGoingToWork.com", 99872),
        std::runtime_error
    );
}
TEST(StreamSimpleTest, ReadPastEOF)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    char data[16];
    stream.read(data,16);
    ASSERT_THROW(stream.read(data,16), std::runtime_error);
}
TEST(StreamSimpleTest, ReadFail)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);
    close(socket);

    char data[16];
    ASSERT_THROW(stream.read(data,16), std::runtime_error);
}
TEST(StreamSimpleTest, WriteNormal)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    {
        StreamSimple stream(socket);

        char data[16] = "12345678";
        stream.write(data,8);
    }
    {
        std::ifstream   test("test/data/StreamSimpleTest-WriteNormal");
        std::string     line;
        std::getline(test, line);
        ASSERT_EQ("12345678", line);
    }
    unlink("test/data/StreamSimpleTest-WriteNormal");
}
TEST(StreamSimpleTest, WriteNormalWithContinue)
{
    SocketSetUp     setupSockets;

    MOCK_SYS(writeMYSQLWrapper, [](int socket, void const* buffer, std::size_t len) -> ssize_t
    {
        static bool firstTime = true;
        if (firstTime)
        {
            firstTime = false;
            errno = EINTR;
            return -1L;
        }
        return ::write(socket, buffer, len);
    });
    int         socket  = open("test/data/StreamSimpleTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    {
        StreamSimple stream(socket);

        char data[16] = "12345678";
        stream.write(data,8);
    }
    {
        std::ifstream   test("test/data/StreamSimpleTest-WriteNormal");
        std::string     line;
        std::getline(test, line);
        ASSERT_EQ("12345678", line);
    }
    unlink("test/data/StreamSimpleTest-WriteNormal");
}
TEST(StreamSimpleTest, WriteFail)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    StreamSimple stream(socket);
    close(socket);

    char data[16] = "12345678";
    ASSERT_THROW(stream.write(data,8), std::runtime_error);
    unlink("test/data/StreamSimpleTest-WriteNormal");
}
TEST(StreamSimpleTest, WriteFailMocked)
{
    SocketSetUp     setupSockets;

    MOCK_SYS(writeMYSQLWrapper, [](int socket, void const* buffer, std::size_t len) {return 0;});
    int         socket  = open("test/data/StreamSimpleTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    StreamSimple stream(socket);

    char data[16] = "12345678";
    ASSERT_THROW(stream.write(data,8), std::runtime_error);
    unlink("test/data/StreamSimpleTest-WriteNormal");
}
TEST(StreamSimpleTest, WriteToSlowStreamToGetEAGAIN)
{
#ifdef __WINNT__
    GTEST_SKIP() << "Windows does not support non blocking pipes";
#else
    SocketSetUp     setupSockets;

    int sysres;
    int const blocks   = 4;
    int const actCount = 46;
    int const bufSize  = 524288 / (blocks * actCount);
    std::vector<char> testData(bufSize);
    std::vector<char> resultData(bufSize);

    for(int loop=0; loop < bufSize; ++loop) {
        testData[loop] = ('0' + (loop % 10));
    }

    int pipes[2];
    sysres = ::pipe(pipes);
    ASSERT_EQ(0, sysres);
    int flags = ::fcntl(pipes[1], F_GETFL, 0);
    ASSERT_NE(-1, flags);
    sysres = ::fcntl(pipes[1], F_SETFL, flags | O_NONBLOCK);
    ASSERT_NE(-1, sysres);

    std::thread slowStream([&resultData, &pipes]() {
        for(int blockLoop = 0; blockLoop < blocks; ++blockLoop)
        {
            sleep(1);
            for(int loop=0;loop < actCount; ++loop)
            {
                int readSoFar = 0;
                while(readSoFar != bufSize)
                {
                    int nextRead = ::read(pipes[0], &resultData[readSoFar], bufSize - readSoFar);
                    if (nextRead == -1 && errno == EAGAIN)
                    {
                        continue;
                    }
                    ASSERT_NE(-1, nextRead);
                    readSoFar += nextRead;
                }
            }
        }
    });

    StreamSimple stream(pipes[1]);
    for(int blockLoop = 0; blockLoop < blocks; ++blockLoop)
    {
        for(int loop=0;loop < actCount; ++loop)
        {
            stream.write(&testData[0], bufSize);
        }
    }
    slowStream.join();

    for(int loop = 0; loop < bufSize; ++loop) {
        ASSERT_EQ(testData[loop], resultData[loop]);
    }
    ::close(pipes[0]);
    //::close(pipes[1]); // closed by stream
#endif
}

class ServerSocket
{
    SocketSetUp     setupSockets;

    int socketId;
    public:
        static constexpr int maxConnectionBacklog = 5;
        ServerSocket(int port, int maxWaitingConnections = maxConnectionBacklog)
            : socketId(::socket(PF_INET, SOCK_STREAM, 0))
        {
            if (socketId == -1)
            {
                throw std::runtime_error(buildErrorMessage("Test:ServerSocket", "socket", "failed: ", systemErrorMessage()));
            }
            struct sockaddr_in    serverAddr = {};
            serverAddr.sin_family       = AF_INET;
            serverAddr.sin_port         = htons(port);
            serverAddr.sin_addr.s_addr  = INADDR_ANY;

            if (::bind(socketId, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) != 0)
            {
                ::close(socketId);
                throw std::runtime_error(buildErrorMessage("Test::ServerSocket", "bind", "failed: ", systemErrorMessage()));
            }

            if (::listen(socketId, maxWaitingConnections) != 0)
            {
                ::close(socketId);
                throw std::runtime_error(buildErrorMessage("Test::ServerSocket", "listen", "failed: ", systemErrorMessage()));
            }
        }
        ~ServerSocket()
        {
            ::close(socketId);
        }
        int accept()
        {
            int newSocket = ::accept(socketId, nullptr, nullptr);
            if (newSocket == -1)
            {
                throw std::runtime_error(buildErrorMessage("Test::ServerSocket", "accept", "failed: ", systemErrorMessage()));
            }
            return newSocket;
        }
};

class ConnectSocket
{
    SocketSetUp     setupSockets;

    int socketId;
    public:
        ConnectSocket(std::string const& host, int port)
            : socketId(::socket(PF_INET, SOCK_STREAM, 0))
        {
            if (socketId == -1)
            {
                throw std::runtime_error(buildErrorMessage("Test:ConnectSocket", "socket", "failed: ", systemErrorMessage()));
            }

            struct hostent* serv = ::gethostbyname(host.c_str());
            if (serv == nullptr)
            {
                ::close(socketId);
                throw std::runtime_error(buildErrorMessage("Test:ConnectSocket", "gethostbyname", "failed: ", systemErrorMessage()));
            }

            struct sockaddr_in serverAddr{};
            serverAddr.sin_family       = AF_INET;
            serverAddr.sin_port         = htons(port);
            char* src = reinterpret_cast<char*>(serv->h_addr);
            char* dst = reinterpret_cast<char*>(&serverAddr.sin_addr.s_addr);
            std::copy(src, src + serv->h_length, dst);

            if (::connect(socketId, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) != 0)
            {
                ::close(socketId);
                throw std::runtime_error(buildErrorMessage("Test:ConnectSocket", "connect", "failed: ", systemErrorMessage()));
            }
        }
        ~ConnectSocket()
        {
            ::close(socketId);
        }

        int getSocketId()
        {
            return socketId;
        }
};

TEST(StreamSimpleTest, OpenSSLConnection)
{
    SocketSetUp     setupSockets;

    int port    = 2022;

    std::thread sslServer([port]() {
        ThorsAnvil::ThorsIO::SSLMethod  method(ThorsAnvil::ThorsIO::SSLMethodType::Server);
        ThorsAnvil::ThorsIO::SSLctx     ctx(method, "test/data/cert.pem", "test/data/key.pem");
        ServerSocket                    server(port);
        int                             client  = server.accept();
        ThorsAnvil::ThorsIO::SSLObj     sslConnection(ctx, client);

        sslConnection.accept();
        sslConnection.write(client, "1234", 4);
    });

    sleep(2);

    ThorsAnvil::ThorsIO::SSLMethod  method(ThorsAnvil::ThorsIO::SSLMethodType::Client);
    ThorsAnvil::ThorsIO::SSLctx     ctx(method);
    ConnectSocket                   connection("127.0.0.1", port);
    ThorsAnvil::ThorsIO::SSLObj     sslConnection(ctx, connection.getSocketId());
    sslConnection.doConnect();

    char buffer[10] = {0};
    sslConnection.read(connection.getSocketId(), buffer, 4);

    ASSERT_EQ(std::string("1234"), buffer);

    sslServer.join();
}

TEST(StreamSimpleTest, startNewConversationNothingTrue)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.startNewConversation(true);
}
TEST(StreamSimpleTest, startNewConversationNothingFalse)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.startNewConversation(false);
}
TEST(StreamSimpleTest, flushNothing)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.flush();
}
TEST(StreamSimpleTest, resetNothing)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.reset();
}
TEST(StreamSimpleTest, dropNothing)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.drop();
}
TEST(StreamSimpleTest, isEmptyNothing)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    EXPECT_EQ(stream.isEmpty(), true);
}
TEST(StreamSimpleTest, readRemainingDataNothing)
{
    SocketSetUp     setupSockets;

    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    EXPECT_EQ(stream.readRemainingData(), "");
}
