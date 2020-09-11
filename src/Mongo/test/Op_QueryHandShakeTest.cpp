
#include <gtest/gtest.h>
#include "Op_QueryHandShake.h"
#include "ThorSerialize/JsonThor.h"
#include <sstream>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_QueryHandShakeTest, CreateDriver)
{
    Driver      driver;

    std::stringstream stream;
    stream << ThorsAnvil::Serialize::jsonExporter(driver);

    std::string output = stream.str();
    output.erase(std::remove_if(std::begin(output), std::end(output), [](auto x){return std::isspace(x);}), std::end(output));

    EXPECT_EQ(output, R"({"name":"ThorsAnvil::Mongo::Driver","version":"v1.0"})");
}

TEST(Op_QueryHandShakeTest, CreateOS)
{
    OS      os;

    std::stringstream stream;
    stream << ThorsAnvil::Serialize::jsonExporter(os);

    std::string output = stream.str();
    output.erase(std::remove_if(std::begin(output), std::end(output), [](auto x){return std::isspace(x);}), std::end(output));
    auto findVersion = output.find("version");
    if (findVersion != std::string::npos)
    {
        findVersion += 10;
        auto findeEndVersion = output.find('"', findVersion);
        std::string versionInfo = output.substr(findVersion, (findeEndVersion - findVersion));
        output.erase(findVersion, (findeEndVersion - findVersion));
    }

#ifdef   __APPLE__
    EXPECT_EQ(output, R"({"type":"Darwin","name":"MacOSX","architecture":"x86_64","version":""})");
#else
    std::cerr << output << "\n";
    EXPECT_EQ(output, "Please Add a test here");
#endif
}

TEST(Op_QueryHandShakeTest, CreateClient)
{
    Client      client("UnitTest");

    std::stringstream stream;
    stream << ThorsAnvil::Serialize::jsonExporter(client);

    std::string output = stream.str();
    output.erase(std::remove_if(std::begin(output), std::end(output), [](auto x){return std::isspace(x);}), std::end(output));

    auto findOS = output.find("os");
    if (findOS != std::string::npos)
    {
        findOS += 5;
        auto findEndOS = output.find('}', findOS);
        output.erase(findOS, (findEndOS - findOS));
    }
    EXPECT_EQ(output, R"({"application":"UnitTest","driver":{"name":"ThorsAnvil::Mongo::Driver","version":"v1.0"},"os":{},"platform":"ThorDB-Build"})");
}

TEST(Op_QueryHandShakeTest, CreateHandShake)
{
    HandShake   handShake("TheHandShakeTest");

    std::stringstream stream;
    stream << ThorsAnvil::Serialize::jsonExporter(handShake);

    std::string output = stream.str();
    output.erase(std::remove_if(std::begin(output), std::end(output), [](auto x){return std::isspace(x);}), std::end(output));

    auto findOS = output.find("\"os\"");
    if (findOS != std::string::npos)
    {
        findOS += 6;
        auto findEndOS = output.find('}', findOS);
        output.erase(findOS, (findEndOS - findOS));
    }
    // TODO: saslSupportedMechs Needs to be fixed.
    // TODO: hostInfo           Needs to be fixed.
    EXPECT_EQ(output, R"({"isMaster":true,"saslSupportedMechs":"thor.loki","hostInfo":"BatCave.local:27017","client":{"application":"TheHandShakeTest","driver":{"name":"ThorsAnvil::Mongo::Driver","version":"v1.0"},"os":{},"platform":"ThorDB-Build"}})");
}
TEST(Op_QueryHandShakeTest, CreateOpQueryHandShake)
{
    MsgHeader::messageIdSetForTest(11495306);
    Op_QueryHandShake   handshake("Application");

    std::stringstream   stream;
    stream << make_hr(handshake);

    std::string output = stream.str();
    output.erase(std::remove_if(std::begin(output), std::end(output), [](auto x){return std::isspace(x);}), std::end(output));


    auto findOS = output.find("\"os\"");
    if (findOS != std::string::npos)
    {
        findOS += 6;
        auto findEndOS = output.find('}', findOS);
        output.erase(findOS, (findEndOS - findOS));
    }

    EXPECT_EQ(output, R"(messageLength:349requestID:11495306responseTo:0opCode:2004emptyfullCollectionName:admin.$cmdnumberToSkip:0numberToReturn:1query:{"isMaster":true,"saslSupportedMechs":"thor.loki","hostInfo":"BatCave.local:27017","client":{"application":"Application","driver":{"name":"ThorsAnvil::Mongo::Driver","version":"v1.0"},"os":{},"platform":"ThorDB-Build"}}returnFieldsSelector:{})");
}

