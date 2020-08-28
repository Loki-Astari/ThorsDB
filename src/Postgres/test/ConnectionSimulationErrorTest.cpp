#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "ConectReader.h"
#include "PackageBuffer.h"
#include "PostgresStream.h"
#include "Messages.h"
#include "test/MockStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::DB::Postgres::Connection;
using ThorsAnvil::DB::Postgres::ConectReader;
using ThorsAnvil::DB::Postgres::ConectWriter;
using ThorsAnvil::DB::Postgres::PackageBuffer;
using ThorsAnvil::DB::Postgres::PostgresStream;

TEST(ConnectionSimulationErrorTest, UnknownResposeCode)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "E"
                             "\x00\x00\x00\x87"     // size
                             "S" "S 001\0"          // 7
                             "C" "C 002\0"          // 7    14
                             "M" "M 003\0"          // 7    21
                             "D" "D 004\0"          // 7    28
                             "H" "H 005\0"          // 7    35
                             "P" "P 006\0"          // 7    42
                             "p" "p 007\0"          // 7    49
                             "q" "q 008\0"          // 7    56
                             "W" "W 009\0"          // 7    63
                             "s" "s 010\0"          // 7    70
                             "t" "t 011\0"          // 7    77
                             "c" "c 012\0"          // 7    84
                             "d" "d 013\0"          // 7    91
                             "n" "n 014\0"          // 7    98
                             "F" "F 015\0"          // 7    105
                             "L" "L 016\0"          // 7    112
                             "R" "R 017\0"          // 7    119
                             "Z" "Z Invalid\0"      // 11   130
                             "\0";                  // 1    131

    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);
    bool            fail = false;

    try
    {
        Connection      connection("test", "testPassword", "test", {}, reader, writer);
    }
    catch(std::runtime_error const& e)
    {
        fail = true;
        std::string message  = e.what();
    }
    ASSERT_EQ(true, fail);
}


