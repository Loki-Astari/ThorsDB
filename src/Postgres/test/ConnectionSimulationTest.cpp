
#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "ConectReader.h"
#include "PackageBuffer.h"
#include "PostgresStream.h"
#include "Messages.h"
#include "ThorsLogging/ThorsLogging.h"
#include "test/MockStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::DB::Postgres::Connection;
using ThorsAnvil::DB::Postgres::ConectReader;
using ThorsAnvil::DB::Postgres::ConectWriter;
using ThorsAnvil::DB::Postgres::PackageBuffer;
using ThorsAnvil::DB::Postgres::PostgresStream;

TEST(ConnectionSimulationTest, AuthenticationKerberosV5)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "R"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\x02";    // AuthenticationKerberosV5 code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);
    bool            fail = false;

    try
    {
        Connection      connection("test", "testPassword", "test", {}, reader, writer);
    }
    catch(std::exception const& e)
    {
        fail = true;
        std::string message = e.what();
        std::size_t find    = message.find("Kerberos");

        ASSERT_NE(std::string::npos, find);
    }
    ASSERT_EQ(true, fail);
}
TEST(ConnectionSimulationTest, AuthenticationCleartextPassword)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "R"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\x03";    // AuthenticationCleartextPassword code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);
    bool            fail = false;

    try
    {
        Connection      connection("test", "testPassword", "test", {}, reader, writer);
    }
    catch(std::exception const& e)
    {
        fail = true;
        std::string message = e.what();
        std::size_t find    = message.find("Cleartext Password:");

        ASSERT_NE(std::string::npos, find);
    }
    ASSERT_EQ(true, fail);
}
TEST(ConnectionSimulationTest, AuthenticationSCMCredential)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "R"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\x06";    // AuthenticationSCMCredential code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);
    bool            fail = false;

    try
    {
        Connection      connection("test", "testPassword", "test", {}, reader, writer);
    }
    catch(std::exception const& e)
    {
        fail = true;
        std::string message = e.what();
        std::size_t find    = message.find("SCM Credential:");

        ASSERT_NE(std::string::npos, find);
    }
    ASSERT_EQ(true, fail);
}
TEST(ConnectionSimulationTest, AuthenticationGSS)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "R"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\x07";    // AuthenticationGSS code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);
    bool            fail = false;

    try
    {
        Connection      connection("test", "testPassword", "test", {}, reader, writer);
    }
    catch(std::exception const& e)
    {
        fail = true;
        std::string message = e.what();
        std::size_t find    = message.find("GSS:");

        ASSERT_NE(std::string::npos, find);
    }
    ASSERT_EQ(true, fail);
}
TEST(ConnectionSimulationTest, AuthenticationSSPI)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "R"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\x09";    // AuthenticationSSPI code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);
    bool            fail = false;

    try
    {
        Connection      connection("test", "testPassword", "test", {}, reader, writer);
    }
    catch(std::exception const& e)
    {
        fail = true;
        std::string message = e.what();
        std::size_t find    = message.find("SSPI:");

        ASSERT_NE(std::string::npos, find);
    }
    ASSERT_EQ(true, fail);
}
TEST(ConnectionSimulationTest, AuthenticationGSSContinue)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "R"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\x08";    // AuthenticationGSSContinue code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);
    bool            fail = false;

    try
    {
        Connection      connection("test", "testPassword", "test", {}, reader, writer);
    }
    catch(std::exception const& e)
    {
        fail = true;
        std::string message = e.what();
        std::size_t find    = message.find("GSS Continue:");

        ASSERT_NE(std::string::npos, find);
    }
    ASSERT_EQ(true, fail);
}

TEST(ConnectionSimulationTest, UnknownResposeCode)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "R"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\xFF";    // AuthenticationGSSContinue code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);

    ASSERT_THROW(
        Connection      connection("test", "testPassword", "test", {}, reader, writer),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(ConnectionSimulationTest, UnknownMessageType)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "Z"
                             "\x00\x00\x00\x08"     // size
                             "\x00\x00\x00\xFF";    // AuthenticationGSSContinue code required.
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);

    ASSERT_THROW(
        Connection      connection("test", "testPassword", "test", {}, reader, writer),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(ConnectionSimulationTest, InvalidMessageType)
{
    // area that we write to
    unsigned char   result[1024];
    // Simulated replies from the server
    char const      data[] = "1"
                             "\x00\x00\x00\x04";     // size
    MockStream      buffer(data, sizeof(data), result);
    PackageBuffer   package(buffer);
    ConectWriter    writer(package);
    ConectReader    reader(package);

    ASSERT_THROW(
        Connection      connection("test", "testPassword", "test", {}, reader, writer),
        ThorsAnvil::Logging::CriticalException
    );
}
TEST(ConnectionSimulationTest, PrintStartupMessage)
{
    ThorsAnvil::DB::Postgres::StartupMessage message("TheUser", "ADBWithData", {{"Map", "Key"}});
    std::stringstream output;
    output << message;

    std::string outputMessage = output.str();

    std::size_t nameMess = outputMessage.find("StartupMessage");
    std::size_t username = outputMessage.find("TheUser");
    std::size_t database = outputMessage.find("ADBWithData");

    ASSERT_NE(std::string::npos, nameMess);
    ASSERT_NE(std::string::npos, username);
    ASSERT_NE(std::string::npos, database);
}

