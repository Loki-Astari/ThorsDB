
#include "StreamSimple.h"
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

using ThorsAnvil::SQL::StreamSimple;

TEST(StreamSimpleTest, ReadNormal)
{
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
TEST(StreamSimpleTest, WriteFail)
{
    int         socket  = open("test/data/StreamSimpleTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    StreamSimple stream(socket);
    close(socket);

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