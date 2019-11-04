
#include "StreamSimple.h"
#include "SSLUtil.h"
#include "coverage/ThorMock.h"
#include <fstream>
#include <thread>
#include <gtest/gtest.h>

#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


using ThorsAnvil::SQL::StreamSimple;
using ThorsAnvil::errorMsg;

TEST(StreamSimpleTest, ReadNormal)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    char data[16];
    stream.read(data,16);

    ASSERT_EQ(std::string(data, data + 16), std::string("1234567890ABCDEF"));
}
TEST(StreamSimpleTest, ReadInterupt)
{
    MOCK_SYS(readMYSQLWrapper, [](int socket, void* buffer, std::size_t len)
    {
        static bool firstTime = true;
        if (firstTime)
        {
            firstTime = false;
            errno = EINTR;
            return -1L;
        }
        return ::read(socket, buffer, len);
    });
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    char data[16];
    stream.read(data,16);

    ASSERT_EQ(std::string(data, data + 16), std::string("1234567890ABCDEF"));
}
TEST(StreamSimpleTest, ReadGoodHost)
{
    StreamSimple stream("google.com", 80);
}
TEST(StreamSimpleTest, NonBlockingFail)
{
    MOCK_SYS(fcntlMYSQLWrapper, [](int, int, int){return -1;});
    auto test = []()
    {
        StreamSimple stream("google.com", 80, true);
    };

    EXPECT_THROW(
        test(),
        std::domain_error
    );
}
TEST(StreamSimpleTest, NonBlockingOK)
{
    MOCK_SYS(fcntlMYSQLWrapper, [](int, int, int){return 0;});
    StreamSimple stream("google.com", 80, true);
}
TEST(StreamSimpleTest, GetCoverageOnTheWrapper)
{
    fcntlMYSQLWrapper(-1, 0, 0);
}
TEST(StreamSimpleTest, ReadGoodHostBadPort)
{
    // ::connect should fail
    ASSERT_THROW(
        StreamSimple stream("127.0.0.1", 1),
        std::runtime_error
    );
}
TEST(StreamSimpleTest, ReadGoodHostSocketFail)
{
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
}
TEST(StreamSimpleTest, ReadFromSlowStreamToGetEAGAIN)
{
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
}
TEST(StreamSimpleTest, ReadBadHost)
{
    ASSERT_THROW(
        StreamSimple stream("BadHostNotGoingToWork.com", 99872),
        std::runtime_error
    );
}
TEST(StreamSimpleTest, ReadPastEOF)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    char data[16];
    stream.read(data,16);
    ASSERT_THROW(stream.read(data,16), std::runtime_error);
}
TEST(StreamSimpleTest, ReadFail)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);
    close(socket);

    char data[16];
    ASSERT_THROW(stream.read(data,16), std::runtime_error);
}
TEST(StreamSimpleTest, WriteNormal)
{
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
    MOCK_SYS(writeMYSQLWrapper, [](int socket, void const* buffer, std::size_t len)
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
    int         socket  = open("test/data/StreamSimpleTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    StreamSimple stream(socket);
    close(socket);

    char data[16] = "12345678";
    ASSERT_THROW(stream.write(data,8), std::runtime_error);
    unlink("test/data/StreamSimpleTest-WriteNormal");
}
TEST(StreamSimpleTest, WriteFailMocked)
{
    MOCK_SYS(writeMYSQLWrapper, [](int socket, void const* buffer, std::size_t len) {return 0;});
    int         socket  = open("test/data/StreamSimpleTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    StreamSimple stream(socket);

    char data[16] = "12345678";
    ASSERT_THROW(stream.write(data,8), std::runtime_error);
    unlink("test/data/StreamSimpleTest-WriteNormal");
}
TEST(StreamSimpleTest, WriteToSlowStreamToGetEAGAIN)
{
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
}

class ServerSocket
{
    int socketId;
    public:
        static constexpr int maxConnectionBacklog = 5;
        ServerSocket(int port, int maxWaitingConnections = maxConnectionBacklog)
            : socketId(::socket(PF_INET, SOCK_STREAM, 0))
        {
            if (socketId == -1)
            {
                throw std::runtime_error(errorMsg("Test:ServerSocket: socket() failed: ", strerror(errno)));
            }
            struct sockaddr_in    serverAddr = {};
            serverAddr.sin_family       = AF_INET;
            serverAddr.sin_port         = htons(port);
            serverAddr.sin_addr.s_addr  = INADDR_ANY;

            if (::bind(socketId, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) != 0)
            {
                ::close(socketId);
                throw std::runtime_error(errorMsg("Test::ServerSocket: bind() failed: ", strerror(errno)));
            }

            if (::listen(socketId, maxWaitingConnections) != 0)
            {
                ::close(socketId);
                throw std::runtime_error(errorMsg("Test::ServerSocket: listen() failed: ", strerror(errno)));
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
                throw std::runtime_error(errorMsg("Test::ServerSocket: accept() failed: ", strerror(errno)));
            }
            return newSocket;
        }
};

class ConnectSocket
{
    int socketId;
    public:
        ConnectSocket(std::string const& host, int port)
            : socketId(::socket(PF_INET, SOCK_STREAM, 0))
        {
            if (socketId == -1)
            {
                throw std::runtime_error(errorMsg("Test:ConnectSocket: socket() failed: ", strerror(errno)));
            }

            struct hostent* serv = ::gethostbyname(host.c_str());
            if (serv == nullptr)
            {
                ::close(socketId);
                throw std::runtime_error(errorMsg("Test:ConnectSocket: gethostbyname() failed: ", strerror(errno)));
            }

            struct sockaddr_in serverAddr{};
            serverAddr.sin_family       = AF_INET;
            serverAddr.sin_port         = htons(port);
            bcopy((char *)serv->h_addr, (char *)&serverAddr.sin_addr.s_addr, serv->h_length);

            if (::connect(socketId, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) != 0)
            {
                ::close(socketId);
                throw std::runtime_error(errorMsg("Test:ConnectSocket: connect() failed: ", strerror(errno)));
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
    int port    = 2022;

    std::thread sslServer([port]() {
        ThorsAnvil::SQL::SSLMethod  method(ThorsAnvil::SQL::SSLMethodType::Server);
        ThorsAnvil::SQL::SSLctx     ctx(method, "test/data/cert.pem", "test/data/key.pem");
        ServerSocket                server(port);
        int                         client  = server.accept();
        ThorsAnvil::SQL::SSLObj     sslConnection(ctx, client);

        sslConnection.accept();
        sslConnection.write("1234", 4);
    });

    sleep(2);

    ThorsAnvil::SQL::SSLMethod  method(ThorsAnvil::SQL::SSLMethodType::Client);
    ThorsAnvil::SQL::SSLctx     ctx(method);
    ConnectSocket               connection("127.0.0.1", port);
    ThorsAnvil::SQL::SSLObj     sslConnection(ctx, connection.getSocketId());
    sslConnection.connect();

    char buffer[10] = {0};
    sslConnection.read(buffer, 4);

    ASSERT_EQ(std::string("1234"), buffer);

    sslServer.join();
}

TEST(StreamSimpleTest, startNewConversationNothingTrue)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.startNewConversation(true);
}
TEST(StreamSimpleTest, startNewConversationNothingFalse)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.startNewConversation(false);
}
TEST(StreamSimpleTest, flushNothing)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.flush();
}
TEST(StreamSimpleTest, resetNothing)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.reset();
}
TEST(StreamSimpleTest, dropNothing)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    stream.drop();
}
TEST(StreamSimpleTest, isEmptyNothing)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    EXPECT_EQ(stream.isEmpty(), true);
}
TEST(StreamSimpleTest, readRemainingDataNothing)
{
    int         socket  = open("test/data/StreamSimpleTest-ReadNormal", O_RDONLY);
    StreamSimple stream(socket);

    EXPECT_EQ(stream.readRemainingData(), "");
}
