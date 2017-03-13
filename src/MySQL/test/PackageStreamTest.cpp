
#include "PackageStream.h"
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

#include <gtest/gtest.h>

using ThorsAnvil::MySQL::MySQLStream;

TEST(PackageStreamTest, ReadNormal)
{
    int         socket  = open("test/data/PackageStreamTest-ReadNormal", O_RDONLY);
    MySQLStream stream(socket);

    char data[16];
    stream.read(data,16);

    ASSERT_EQ(std::string(data, data + 16), std::string("1234567890ABCDEF"));
}
TEST(PackageStreamTest, ReadBadHost)
{
    ASSERT_THROW(
        MySQLStream stream("BadHostNotGoingToWork.com", 99872),
        std::runtime_error
    );
}
TEST(PackageStreamTest, ReadPastEOF)
{
    int         socket  = open("test/data/PackageStreamTest-ReadNormal", O_RDONLY);
    MySQLStream stream(socket);

    char data[16];
    stream.read(data,16);
    ASSERT_THROW(stream.read(data,16), std::runtime_error);
}
TEST(PackageStreamTest, ReadFail)
{
    int         socket  = open("test/data/PackageStreamTest-ReadNormal", O_RDONLY);
    MySQLStream stream(socket);
    close(socket);

    char data[16];
    ASSERT_THROW(stream.read(data,16), std::runtime_error);
}
TEST(PackageStreamTest, WriteNormal)
{
    int         socket  = open("test/data/PackageStreamTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    {
        MySQLStream stream(socket);

        char data[16] = "12345678";
        stream.write(data,8);
    }
    {
        std::ifstream   test("test/data/PackageStreamTest-WriteNormal");
        std::string     line;
        std::getline(test, line);
        ASSERT_EQ("12345678", line);
    }
    unlink("test/data/PackageStreamTest-WriteNormal");
}
TEST(PackageStreamTest, WriteFail)
{
    int         socket  = open("test/data/PackageStreamTest-WriteNormal", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
    MySQLStream stream(socket);
    close(socket);

    char data[16] = "12345678";
    ASSERT_THROW(stream.write(data,8), std::runtime_error);
    unlink("test/data/PackageStreamTest-WriteNormal");
}

