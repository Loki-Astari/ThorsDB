
#include <gtest/gtest.h>
#include "CmdAdm_Auth.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/CustomSerialization.h"
#include "ThorsCrypto/scram.h"
#include "ThorsIOUtil/Format.h"
#include <sstream>
#include <cstdint>
//#include <format>
#include <stdio.h>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(HandShakeTest, CreateDriver)
{
    Driver      driver;

    std::stringstream stream;
    stream << ThorsAnvil::Serialize::jsonExporter(driver);

    std::string output = stream.str();
    output.erase(std::remove_if(std::begin(output), std::end(output), [](auto x){return std::isspace(x);}), std::end(output));

    EXPECT_EQ(output, R"({"name":"ThorsAnvil::Mongo::Driver","version":"v1.0"})");
}

std::string getSystemInfo(std::string const& command)
{
    char result[100] = {0};
    FILE* f = popen(command.c_str(), "r");
    fscanf(f, "%s", result);
    pclose(f);

    return result;
}

template<typename... Args>
std::string makeFormat(char const* fmt, Args const&... args)
{
    std::stringstream stream;
    stream << ThorsAnvil::IOUtil::make_cppformat(fmt, args...);

    return stream.str();
}

std::string createSizeString(std::uint32_t value)
{
    char* begin = reinterpret_cast<char*>(&value);
    char* end   = begin + 4;
    return std::string(begin, end);
}

TEST(HandShakeTest, CreateOS)
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

    using namespace std::string_literals;
    std::string typeVal = getSystemInfo("uname -s | awk -F- '{print $1}'");
    std::string archVal = getSystemInfo("uname -m");
#if defined(__APPLE__)
    std::string osVal   = getSystemInfo("sw_vers -productName");
#elif defined(__linux__)
    std::string osVal   = getSystemInfo(R"(awk -F\" '/NAME/{print $2}' /etc/os-release)");
#elif defined(__WINNT__)
    std::string osVal   = getSystemInfo("uname -o");
#endif
    std::string vers    = "";
    std::string expected = makeFormat(R"({"type":"%s","name":"%s","architecture":"%s","version":"%s"})", typeVal.c_str(), osVal.c_str(), archVal.c_str(), vers.c_str());

#ifndef __WINNT__
    // TODO Fix for windows.
    EXPECT_EQ(output, expected);
#endif
}

TEST(HandShakeTest, CreateApplication)
{
    Application application("TheLongWayDown");

    std::stringstream   stream;
    stream <<  ThorsAnvil::Serialize::jsonExporter(application);

    std::string output = stream.str();
    output.erase(std::remove_if(std::begin(output), std::end(output), [](auto x){return std::isspace(x);}), std::end(output));

    EXPECT_EQ(output, R"({"name":"TheLongWayDown"})");
}

TEST(HandShakeTest, CreateClient)
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
    EXPECT_EQ(output, R"({"application":{"name":"UnitTest"},"driver":{"name":"ThorsAnvil::Mongo::Driver","version":"v1.0"},"os":{}})");
}

TEST(HandShakeTest, CreateHandShake)
{
    // We have a Mongo Shell Handshake object.
    // We should be able to create the equivalent without
    // Much effort.
#if 0
RAW Data
51 01 00 00 00 00 00 00   ?...?...Q.......
00 00 00 00 d4 07 00 00 00 00 00 00 61 64 6d 69   ............admi
6e 2e 24 63 6d 64 00 00 00 00 00 01 00 00 00 2a   n.$cmd.........*
01 00 00 10 69 73 4d 61 73 74 65 72 00 01 00 00   ....isMaster....
00 02 73 61 73 6c 53 75 70 70 6f 72 74 65 64 4d   ..saslSupportedM
65 63 68 73 00 0a 00 00 00 74 68 6f 72 2e 6c 6f   echs.....thor.lo
6b 69 00 02 68 6f 73 74 49 6e 66 6f 00 14 00 00   ki..hostInfo....
00 42 61 74 43 61 76 65 2e 6c 6f 63 61 6c 3a 32   .BatCave.local:2
37 30 31 37 00 03 63 6c 69 65 6e 74 00 cb 00 00   7017..client....
00 03 61 70 70 6c 69 63 61 74 69 6f 6e 00 1d 00   ..application...
00 00 02 6e 61 6d 65 00 0e 00 00 00 4d 6f 6e 67   ...name.....Mong
6f 44 42 20 53 68 65 6c 6c 00 00 03 64 72 69 76   oDB Shell...driv
65 72 00 3a 00 00 00 02 6e 61 6d 65 00 18 00 00   er.:....name....
00 4d 6f 6e 67 6f 44 42 20 49 6e 74 65 72 6e 61   .MongoDB Interna
6c 20 43 6c 69 65 6e 74 00 02 76 65 72 73 69 6f   l Client..versio
6e 00 06 00 00 00 34 2e 32 2e 30 00 00 03 6f 73   n.....4.2.0...os
00 56 00 00 00 02 74 79 70 65 00 07 00 00 00 44   .V....type.....D
61 72 77 69 6e 00 02 6e 61 6d 65 00 09 00 00 00   arwin..name.....
4d 61 63 20 4f 53 20 58 00 02 61 72 63 68 69 74   Mac OS X..archit
65 63 74 75 72 65 00 07 00 00 00 78 38 36 5f 36   ecture.....x86_6
34 00 02 76 65 72 73 69 6f 6e 00 07 00 00 00 31   4..version.....2
39 2e 36 2e 30 00 00 00 00                        0.2.0....

Manually Split Data
51 01 00 00     // Size
00 00 00 00     // Request ID
00 00 00 00     // Response TO
d4 07 00 00     // OP_CODE: 2004 => Op_Query
Query
00 00 00 00     // Flag
admin.$cmd      // fullCollectionName
00 00 00 00     // Number to Skip
01 00 00 00     // Number to return
BSON
27 01 00 00     // Size
    10      isMaster                01 00 00 00
    02      saslSupportedMechs      0a 00 00 00     thor.loki
    02      hostInfo                14 00 00 00     BatCave.local:27017
    03      client
    c8 00 00 00
        03      application
        1d 00 00 00
            02      name    0e 00 00 00     MongoDB Shell
        00
        03      driver
        3a 00 00 00
            02      name    18 00 00 00     MongoDB Internal Client
            02      version 06 00 00 00     4.2.0
        00
        03 os
        53 00 00 00
            02      type            07 00 00 00     Darwin
            02      name            06 00 00 00     macOS
            02      architecture    07 00 00 00     x86_64
            02      version         07 00 00 00     20.2.0
        00
    00
00
#endif
#if defined(__APPLE__)
    std::string   typeVal         = getSystemInfo("uname -s");                // Darwin
    std::string   osVal           = getSystemInfo("sw_vers -productName");    // macOS
    std::string   archVal         = getSystemInfo("uname -m");                // x86_64
    std::string   osVerVal        = getSystemInfo("uname -r");                // 20.2.0
#elif defined(__linux__)
    std::string   typeVal         = getSystemInfo("uname -s");                // Linux
    std::string   osVal           = getSystemInfo(R"(awk -F\" '/NAME/{print $2}' /etc/os-release)");    // Ubuntu
    std::string   archVal         = getSystemInfo("uname -m");                // x86_64
    std::string   osVerVal        = getSystemInfo("uname -r");                // 4.15.0-1077-gcp
#elif defined(__WINNT__)
    std::string   typeVal         = "MSYS_NT";
    std::string   osVal           = "Msys";
    std::string   archVal         = "x86_64";
    std::string   osVerVal        = "3.4.7.x86_64";
#else
#error "See above and fix"
#endif

    using namespace std::string_literals;
    std::uint32_t typeSz          = typeVal.size() + 1;
    std::uint32_t osSz            = osVal.size() + 1;
    std::uint32_t archSz          = archVal.size() + 1;
    std::uint32_t osVerSz         = osVerVal.size() + 1;
    std::uint32_t osInfoSize      = 0x53  - 0x1b + typeSz + osSz + archSz + osVerSz;
    std::uint32_t allInfoSize     = 0xc8  - 0x53 + osInfoSize;
    std::uint32_t bsonInfoSize    = 0x127 - 0xc8 + allInfoSize + 26;
    std::uint32_t msgSize         = 0x14e - 0xc8 + allInfoSize + 26;
    std::string   typeSzStr       = createSizeString(typeSz);
    std::string   osSzStr         = createSizeString(osSz);
    std::string   archSzStr       = createSizeString(archSz);
    std::string   osVerSzStr      = createSizeString(osVerSz);
    std::string   osInfoSizeStr   = createSizeString(osInfoSize);
    std::string   allInfoSizeStr  = createSizeString(allInfoSize);
    std::string   bsonInfoSizeStr = createSizeString(bsonInfoSize);
    std::string   msgSizeStr      = createSizeString(msgSize);
    std::string expected =
   //     "\x4e\x01\x00\x00"      // Size
        msgSizeStr +
        "\x00\x00\x00\x00"      // Request Id
        "\x00\x00\x00\x00"      // Response TO
        "\xd4\x07\x00\x00"      // OP_CODE: 2004 => Op_Query
        // Query
        "\x00\x00\x00\x00"      // Flag
        "\x61\x64\x6d\x69\x6e\x2e\x24\x63\x6d\x64\x00"
        "\x00\x00\x00\x00"      // Number to Skip
        "\x01\x00\x00\x00"s     // Number to return
        // BSON
        //"\x27\x01\x00\x00"      // Size
        + bsonInfoSizeStr +
        "\x10\x69\x73\x4d\x61\x73\x74\x65\x72\x00"  "\x01\x00\x00\x00"
        "\x02\x73\x61\x73\x6c\x53\x75\x70\x70\x6f\x72\x74\x65\x64\x4d\x65\x63\x68\x73\x00"  "\x0a\x00\x00\x00"  "\x74\x68\x6f\x72\x2e\x6c\x6f\x6b\x69\x00"
        "\x02\x68\x6f\x73\x74\x49\x6e\x66\x6f\x00"  "\x14\x00\x00\x00"  "\x42\x61\x74\x43\x61\x76\x65\x2e\x6c\x6f\x63\x61\x6c\x3a\x32\x37\x30\x31\x37\x00"
        "\x03\x63\x6c\x69\x65\x6e\x74\x00"s
            //"\xc8\x00\x00\x00"
          + allInfoSizeStr +
                "\x03\x61\x70\x70\x6c\x69\x63\x61\x74\x69\x6f\x6e\x00"
                "\x1d\x00\x00\x00"
                    "\x02\x6e\x61\x6d\x65\x00"  "\x0e\x00\x00\x00"  "\x4d\x6f\x6e\x67\x6f\x44\x42\x20\x53\x68\x65\x6c\x6c\x00"
                    "\x00"
                    "\x03\x64\x72\x69\x76\x65\x72\x00"
                    "\x3a\x00\x00\x00"
                        "\x02\x6e\x61\x6d\x65\x00"              "\x18\x00\x00\x00"  "\x4d\x6f\x6e\x67\x6f\x44\x42\x20\x49\x6e\x74\x65\x72\x6e\x61\x6c\x20\x43\x6c\x69\x65\x6e\x74\x00"
                        "\x02\x76\x65\x72\x73\x69\x6f\x6e\x00"  "\x06\x00\x00\x00"  "\x34\x2e\x32\x2e\x30\x00"
                    "\x00"
                    "\x03\x6f\x73\x00"s
                    // "\x53\x00\x00\x00"
                  + osInfoSizeStr +

                        "\x02\x74\x79\x70\x65\x00"s
                            // "\x07\x00\x00\x00"  "\x44\x61\x72\x77\x69\x6e\x00"  // Darwin
                         +  typeSzStr + typeVal.c_str() + "\x00"s +
                        "\x02\x6e\x61\x6d\x65\x00"s
                            // "\x06\x00\x00\x00"  "\x6d\x61\x63\x4f\x53\x00"      // macOS
                         +  osSzStr + osVal.c_str() + "\x00"s +
                        "\x02\x61\x72\x63\x68\x69\x74\x65\x63\x74\x75\x72\x65\x00"s
                            // "\x07\x00\x00\x00"  "\x78\x38\x36\x5f\x36\x34\x00"  // X86_64
                         +  archSzStr + archVal.c_str() + "\x00"s +
                        "\x02\x76\x65\x72\x73\x69\x6f\x6e\x00"s
                            // "\x07\x00\x00\x00"  "\x32\x30\x2e\x32\x2e\x30\x00"  // 20.2.0
                         +  osVerSzStr + osVerVal.c_str() + "\x00"s +
                    "\x00"
            "\x00"
            "\x04" "compression\x00" "\x0D\x00\x00\x00"
                "\x02" "0\x00" "\x01\x00\x00\x00"  "\x00"
            "\x00"
        "\x00"s;

    Op_MsgHeader::messageIdSetForTest(0);
    CmdAdm_HandShake   handShakeMessage("MongoDB Shell", "MongoDB Internal Client", "4.2.0");

    std::stringstream   stream;
    stream << handShakeMessage;

    EXPECT_EQ(expected, stream.str());
}

TEST(HandShakeTest, ReadHandShakeResponse)
{
    // We have a Mongo Shell Handshake response object.
    // We should be able to read the response object and verify we have all the values.
#if 0
RAW
80 01 00 00 9c 11 00 00   ?...?...........
00 00 00 00 01 00 00 00 08 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00 01 00 00 00 5c 01 00 00   ............\...
08 69 73 6d 61 73 74 65 72 00 01 03 74 6f 70 6f   .ismaster...topo
6c 6f 67 79 56 65 72 73 69 6f 6e 00 2d 00 00 00   logyVersion.-...
07 70 72 6f 63 65 73 73 49 64 00 5f 48 aa 04 2d   .processId._H..-
1d fb 1d d7 05 2e 00 12 63 6f 75 6e 74 65 72 00   ........counter.
00 00 00 00 00 00 00 00 00 10 6d 61 78 42 73 6f   ..........maxBso
6e 4f 62 6a 65 63 74 53 69 7a 65 00 00 00 00 01   nObjectSize.....
10 6d 61 78 4d 65 73 73 61 67 65 53 69 7a 65 42   .maxMessageSizeB
79 74 65 73 00 00 6c dc 02 10 6d 61 78 57 72 69   ytes..l...maxWri
74 65 42 61 74 63 68 53 69 7a 65 00 a0 86 01 00   teBatchSize.....
09 6c 6f 63 61 6c 54 69 6d 65 00 2d 11 af 76 74   .localTime.-..vt
01 00 00 10 6c 6f 67 69 63 61 6c 53 65 73 73 69   ....logicalSessi
6f 6e 54 69 6d 65 6f 75 74 4d 69 6e 75 74 65 73   onTimeoutMinutes
00 1e 00 00 00 10 63 6f 6e 6e 65 63 74 69 6f 6e   ......connection
49 64 00 1e 00 00 00 10 6d 69 6e 57 69 72 65 56   Id......minWireV
65 72 73 69 6f 6e 00 00 00 00 00 10 6d 61 78 57   ersion......maxW
69 72 65 56 65 72 73 69 6f 6e 00 09 00 00 00 08   ireVersion......
72 65 61 64 4f 6e 6c 79 00 00 04 73 61 73 6c 53   readOnly...saslS
75 70 70 6f 72 74 65 64 4d 65 63 68 73 00 2d 00   upportedMechs.-.
00 00 02 30 00 0c 00 00 00 53 43 52 41 4d 2d 53   ...0.....SCRAM-S
48 41 2d 31 00 02 31 00 0e 00 00 00 53 43 52 41   HA-1..1.....SCRA
4d 2d 53 48 41 2d 32 35 36 00 00 01 6f 6b 00 00   M-SHA-256...ok..
00 00 00 00 00 f0 3f 00                           ......?.

Manually Split Data
80 01 00 00     // Size
9c 11 00 00     // Message ID
00 00 00 00     // Response To
01 00 00 00     // Op_Code: 1: Op_Reply
Reply
08 00 00 00     // Flag
00 00 00 00     // Cursor 1
00 00 00 00     // Cursor 2
00 00 00 00     // Starting From
01 00 00 00     // Number Returned
BSON
5c 01 00 00     // Size
    08      ismaster    01
    03      topologyVersion
    2d 00 00 00
        07      processId                   5f 48 aa 04 2d 1d fb 1d d7 05 2e 00
        12      counter                     00 00 00 00 00 00 00 00
    00
    10      maxBsonObjectSize               00 00 00 01
    10      maxMessageSizeBytes             00 6c dc 02
    10      maxWriteBatchSize               a0 86 01 00
    09      localTime                       2d 11 af 76 74 01 00 00
    10      logicalSessionTimeoutMinutes    1e 00 00 00
    10      connectionId                    1e 00 00 00
    10      minWireVersion                  00 00 00 00
    10      maxWireVersion                  09 00 00 00
    08      readOnly                        00
    04      saslSupportedMechs
    2d 00 00 00
        02  0   0c 00 00 00     SCRAM-SHA-1
        02  1   0e 00 00 00     SCRAM-SHA-256
    00
    01      ok                  00 00 00 00 00 00 f0 3f
00
#endif

    std::string input = "\x80\x01\x00\x00\x9c\x11\x00\x00"
                        "\x00\x00\x00\x00\x01\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00"
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x5c\x01\x00\x00"
                        "\x08\x69\x73\x6d\x61\x73\x74\x65\x72\x00\x01\x03\x74\x6f\x70\x6f"
                        "\x6c\x6f\x67\x79\x56\x65\x72\x73\x69\x6f\x6e\x00\x2d\x00\x00\x00"
                        "\x07\x70\x72\x6f\x63\x65\x73\x73\x49\x64\x00\x5f\x48\xaa\x04\x2d"
                        "\x1d\xfb\x1d\xd7\x05\x2e\x00\x12\x63\x6f\x75\x6e\x74\x65\x72\x00"
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\x6d\x61\x78\x42\x73\x6f"
                        "\x6e\x4f\x62\x6a\x65\x63\x74\x53\x69\x7a\x65\x00\x00\x00\x00\x01"
                        "\x10\x6d\x61\x78\x4d\x65\x73\x73\x61\x67\x65\x53\x69\x7a\x65\x42"
                        "\x79\x74\x65\x73\x00\x00\x6c\xdc\x02\x10\x6d\x61\x78\x57\x72\x69"
                        "\x74\x65\x42\x61\x74\x63\x68\x53\x69\x7a\x65\x00\xa0\x86\x01\x00"
                        "\x09\x6c\x6f\x63\x61\x6c\x54\x69\x6d\x65\x00\x2d\x11\xaf\x76\x74"
                        "\x01\x00\x00\x10\x6c\x6f\x67\x69\x63\x61\x6c\x53\x65\x73\x73\x69"
                        "\x6f\x6e\x54\x69\x6d\x65\x6f\x75\x74\x4d\x69\x6e\x75\x74\x65\x73"
                        "\x00\x1e\x00\x00\x00\x10\x63\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e"
                        "\x49\x64\x00\x1e\x00\x00\x00\x10\x6d\x69\x6e\x57\x69\x72\x65\x56"
                        "\x65\x72\x73\x69\x6f\x6e\x00\x00\x00\x00\x00\x10\x6d\x61\x78\x57"
                        "\x69\x72\x65\x56\x65\x72\x73\x69\x6f\x6e\x00\x09\x00\x00\x00\x08"
                        "\x72\x65\x61\x64\x4f\x6e\x6c\x79\x00\x00\x04\x73\x61\x73\x6c\x53"
                        "\x75\x70\x70\x6f\x72\x74\x65\x64\x4d\x65\x63\x68\x73\x00\x2d\x00"
                        "\x00\x00\x02\x30\x00\x0c\x00\x00\x00\x53\x43\x52\x41\x4d\x2d\x53"
                        "\x48\x41\x2d\x31\x00\x02\x31\x00\x0e\x00\x00\x00\x53\x43\x52\x41"
                        "\x4d\x2d\x53\x48\x41\x2d\x32\x35\x36\x00\x00\x01\x6f\x6b\x00\x00"
                        "\x00\x00\x00\x00\x00\xf0\x3f\x00"s;
    std::stringstream   stream(input);

    CmdAdm_HandShakeReply   reply;
    stream >> reply;

    EXPECT_EQ(reply.reply.ok,                   1.0);
    EXPECT_EQ(reply.reply.ismaster,             1);
    EXPECT_EQ(reply.reply.topologyVersion.processId, ThorsAnvil::Serialize::MongoUtility::ObjectID(0x5f48aa04, 0x2d1dfb1dd7L, 0x52e00));
    EXPECT_EQ(reply.reply.topologyVersion.counter,   0x0L);
    EXPECT_EQ(reply.reply.maxBsonObjectSize,    0x1000000);
    EXPECT_EQ(reply.reply.maxMessageSizeBytes,  0x2dc6c00);
    EXPECT_EQ(reply.reply.maxWriteBatchSize,    0x186a0);
    EXPECT_EQ(reply.reply.localTime,            0x17476af112d);
    EXPECT_EQ(reply.reply.logicalSessionTimeoutMinutes, 0x1e);
    EXPECT_EQ(reply.reply.connectionId,         0x1e);
    EXPECT_EQ(reply.reply.minWireVersion,       0);
    EXPECT_EQ(reply.reply.maxWireVersion,       0x9);
    EXPECT_EQ(reply.reply.readOnly,             false);
    ASSERT_EQ(reply.reply.saslSupportedMechs.size(), 2);
    EXPECT_EQ(reply.reply.saslSupportedMechs[0],"SCRAM-SHA-1");
    EXPECT_EQ(reply.reply.saslSupportedMechs[1],"SCRAM-SHA-256");
}

TEST(HandShakeTest, CreateSASLFirstMessage)
{
    // We have a Mongo Shell Handshake object.
    // We should be able to create the equivalent without
    // Much effort.
#if 0
RAW
92 00 00 00 01 00 00 00   ?...?...........
00 00 00 00 dd 07 00 00 01 00 00 00 00 79 00 00   .............y..
00 10 73 61 73 6c 53 74 61 72 74 00 01 00 00 00   ..saslStart.....
02 6d 65 63 68 61 6e 69 73 6d 00 0e 00 00 00 53   .mechanism.....S
43 52 41 4d 2d 53 48 41 2d 32 35 36 00 05 70 61   CRAM-SHA-256..pa
79 6c 6f 61 64 00 2c 00 00 00 00 6e 2c 2c 6e 3d   yload.,....n,,n=
6c 6f 6b 69 2c 72 3d 4a 53 79 52 48 44 37 73 63   loki,r=JSyRHD7sc
39 52 67 44 43 44 7a 4a 4a 4e 56 64 6b 41 32 47   9RgDCDzJJNVdkA2G
6c 53 65 4d 4a 50 56 02 24 64 62 00 05 00 00 00   lSeMJPV.$db.....
74 68 6f 72 00 00 10 44 12 5c                     thor...D.\

Hand generated output
92 00 00 00     // Size
01 00 00 00     // Message ID
00 00 00 00     // Response To
dd 07 00 00     // OP_Code: 2013 Op_MSG
MSG
01 00 00 00     // Flags
00              // Kind 0
BSON
79 00 00 00     // Size
    10      saslStart                       01 00 00 00
    02      mechanism       0e 00 00 00     SCRAM-SHA-256
    05      payload         2c 00 00 00     00      n,,n=loki,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV
    02      $db             05 00 00 00     thor
00
10 44 12 5c     // Checksum
#endif

    // Removing the checksum (I have not implemented the calculation of this.
    // Please add back when it is done.
    std::string expected =  /*"\x92"*/"\x8e" // remove 4 because reduced by four.
                            "\x00\x00\x00\x01\x00\x00\x00"
                            "\x00\x00\x00\x00\xdd\x07\x00\x00" /*"\x01" Flag to Turn on Checksum: Replace by \x00 -> */ "\x00" "\x00\x00\x00\x00\x79\x00\x00"
                            "\x00\x10\x73\x61\x73\x6c\x53\x74\x61\x72\x74\x00\x01\x00\x00\x00"
                            "\x02\x6d\x65\x63\x68\x61\x6e\x69\x73\x6d\x00\x0e\x00\x00\x00\x53"
                            "\x43\x52\x41\x4d\x2d\x53\x48\x41\x2d\x32\x35\x36\x00\x05\x70\x61"
                            "\x79\x6c\x6f\x61\x64\x00\x2c\x00\x00\x00\x00\x6e\x2c\x2c\x6e\x3d"
                            "\x6c\x6f\x6b\x69\x2c\x72\x3d\x4a\x53\x79\x52\x48\x44\x37\x73\x63"
                            "\x39\x52\x67\x44\x43\x44\x7a\x4a\x4a\x4e\x56\x64\x6b\x41\x32\x47"
                            "\x6c\x53\x65\x4d\x4a\x50\x56\x02\x24\x64\x62\x00\x05\x00\x00\x00"
                            "\x74\x68\x6f\x72\x00\x00"s; // \x10\x44\x12\x5c"s;

    Op_MsgHeader::messageIdSetForTest(0x1);
    AuthInit            authInit("thor"s, "SCRAM-SHA-256"s, "n,,n=loki,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV"s);
    Op_MsgAuthInit      authInitMessage(OP_MsgFlag::empty, authInit);

    std::stringstream   stream;
    stream << authInitMessage;

    EXPECT_EQ(expected, stream.str());
}

TEST(HandShakeTest, CreateSASLFirstMessageReplicateGeneration)
{
    ThorsAnvil::Crypto::ScramClientSha256   client("loki"s, [](){return "JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV";});

    EXPECT_EQ("n,,n=loki,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV", client.getFirstMessage());
}

TEST(HandShakeTest, ReadSASLFirstMessageResponse)
{
#if 0
c8 00 00 00 9d 11 00 00   ?...?...........
01 00 00 00 dd 07 00 00 01 00 00 00 00 af 00 00   ................
00 10 63 6f 6e 76 65 72 73 61 74 69 6f 6e 49 64   ..conversationId
00 01 00 00 00 08 64 6f 6e 65 00 00 05 70 61 79   ......done...pay
6c 6f 61 64 00 75 00 00 00 00 72 3d 4a 53 79 52   load.u....r=JSyR
48 44 37 73 63 39 52 67 44 43 44 7a 4a 4a 4e 56   HD7sc9RgDCDzJJNV
64 6b 41 32 47 6c 53 65 4d 4a 50 56 35 6e 39 35   dkA2GlSeMJPV5n95
70 30 77 64 5a 7a 78 6a 50 74 37 7a 79 4c 45 4e   p0wdZzxjPt7zyLEN
66 31 54 6f 38 68 59 54 62 4b 45 51 2c 73 3d 34   f1To8hYTbKEQ,s=4
4c 30 78 41 7a 6e 46 67 75 53 31 72 63 5a 6e 6a   L0xAznFguS1rcZnj
34 57 51 6e 78 4c 65 31 46 35 37 30 53 2b 46 51   4WQnxLe1F570S+FQ
64 4b 7a 4f 77 3d 3d 2c 69 3d 31 35 30 30 30 01   dKzOw==,i=15000.
6f 6b 00 00 00 00 00 00 00 f0 3f 00 28 99 0a 72   ok........?.(..r

c8 00 00 00     // Size
9d 11 00 00     // Message ID
01 00 00 00     // Response To
dd 07 00 00     // Op_Code: 2013 Op_MSG
MSG
01 00 00 00     // Flags
00              // Kind0
BSON
af 00 00 00     // Size
    10      conversationId      01 00 00 00
    08      done                00
    05      payload     75 00 00 00     00      r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,s=4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==,i=15000
    01      ok          00 00 00 00 00 00 f0 3f
00
28 99 0a 72     // Checksum
#endif

    std::string input =     "\xc8\x00\x00\x00\x9d\x11\x00\x00"
                            "\x01\x00\x00\x00\xdd\x07\x00\x00\x01\x00\x00\x00\x00\xaf\x00\x00"
                            "\x00\x10\x63\x6f\x6e\x76\x65\x72\x73\x61\x74\x69\x6f\x6e\x49\x64"
                            "\x00\x01\x00\x00\x00\x08\x64\x6f\x6e\x65\x00\x00\x05\x70\x61\x79"
                            "\x6c\x6f\x61\x64\x00\x75\x00\x00\x00\x00\x72\x3d\x4a\x53\x79\x52"
                            "\x48\x44\x37\x73\x63\x39\x52\x67\x44\x43\x44\x7a\x4a\x4a\x4e\x56"
                            "\x64\x6b\x41\x32\x47\x6c\x53\x65\x4d\x4a\x50\x56\x35\x6e\x39\x35"
                            "\x70\x30\x77\x64\x5a\x7a\x78\x6a\x50\x74\x37\x7a\x79\x4c\x45\x4e"
                            "\x66\x31\x54\x6f\x38\x68\x59\x54\x62\x4b\x45\x51\x2c\x73\x3d\x34"
                            "\x4c\x30\x78\x41\x7a\x6e\x46\x67\x75\x53\x31\x72\x63\x5a\x6e\x6a"
                            "\x34\x57\x51\x6e\x78\x4c\x65\x31\x46\x35\x37\x30\x53\x2b\x46\x51"
                            "\x64\x4b\x7a\x4f\x77\x3d\x3d\x2c\x69\x3d\x31\x35\x30\x30\x30\x01"
                            "\x6f\x6b\x00\x00\x00\x00\x00\x00\x00\xf0\x3f\x00\x28\x99\x0a\x72"s;

    Op_MsgAuthReply         authReplyMessage;

    std::stringstream stream(input);
    stream >> authReplyMessage;

    AuthReply const& authReply = authReplyMessage.getAction();
    EXPECT_EQ(authReply.conversationId,   1);
    EXPECT_EQ(authReply.done,             false);
    EXPECT_EQ(authReply.payload.data,     "r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,s=4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==,i=15000"s);
    EXPECT_EQ(authReply.ok,               1.0);
}

TEST(HandShakeTest, ReadSASLFirstMessageResponseReplicateGeneration)
{
    ThorsAnvil::Crypto::ScramServerSha256   server( "n,,n=loki,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV",
                                                    15000,
                                                    [](){return "5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ";},
                                                    [](ThorsAnvil::Crypto::DBInfoType type, std::string const&)
                                                    {
                                                        return type == ThorsAnvil::Crypto::DBInfoType::Password
                                                            ? "underworldSA0"
                                                            : "4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==";
                                                    });

    EXPECT_EQ("r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,s=4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==,i=15000", server.getFirstMessage());
}

TEST(HandShakeTest, CreateSASLSecondMessage)
{
#if 0
RAW
d8 00 00 00 02 00 00 00   ?...?...........
00 00 00 00 dd 07 00 00 01 00 00 00 00 bf 00 00   ................
00 10 73 61 73 6c 43 6f 6e 74 69 6e 75 65 00 01   ..saslContinue..
00 00 00 05 70 61 79 6c 6f 61 64 00 78 00 00 00   ....payload.x...
00 63 3d 62 69 77 73 2c 72 3d 4a 53 79 52 48 44   .c=biws,r=JSyRHD
37 73 63 39 52 67 44 43 44 7a 4a 4a 4e 56 64 6b   7sc9RgDCDzJJNVdk
41 32 47 6c 53 65 4d 4a 50 56 35 6e 39 35 70 30   A2GlSeMJPV5n95p0
77 64 5a 7a 78 6a 50 74 37 7a 79 4c 45 4e 66 31   wdZzxjPt7zyLENf1
54 6f 38 68 59 54 62 4b 45 51 2c 70 3d 68 38 2b   To8hYTbKEQ,p=h8+
4b 52 48 78 46 48 6b 72 76 43 33 74 36 43 71 36   KRHxFHkrvC3t6Cq6
4b 56 4c 41 74 34 6d 6c 42 50 2f 56 36 4a 72 74   KVLAt4mlBP/V6Jrt
54 4c 4d 4b 76 57 2f 34 3d 10 63 6f 6e 76 65 72   TLMKvW/4=.conver
73 61 74 69 6f 6e 49 64 00 01 00 00 00 02 24 64   sationId......$d
62 00 05 00 00 00 74 68 6f 72 00 00 bd 34 27 d8   b.....thor...4X.

Hand split data
d8 00 00 00         // Size
02 00 00 00         // Message Id
00 00 00 00         // Response To
dd 07 00 00         // Op_CODE: 2013: Op_MSG
MSG
01 00 00 00         // Flag
00                  // Kind0
BSON
bf 00 00 00         // Size
    10      saslContinue    01 00 00 00
    05      payload         78 00 00 00     00      c=biws,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,p=h8+KRHxFHkrvC3t6Cq6KVLAt4mlBP/V6JrtTLMKvW/4=
    10      conversationId  01 00 00 00
    02      $db             05 00 00 00     thor
00
bd 34 27 d8 // Checksum
#endif

                            // don't compute checksum
                            // Removing it from here
    std::string expected =  /*"\xd8 Subtracted 4 to het d4*/ "\xd4" "\x00\x00\x00\x02\x00\x00\x00"
                            "\x00\x00\x00\x00\xdd\x07\x00\x00" /* Disable Flag for check sum "\x01" ->*/ "\x00" "\x00\x00\x00\x00\xbf\x00\x00"
                            "\x00\x10\x73\x61\x73\x6c\x43\x6f\x6e\x74\x69\x6e\x75\x65\x00\x01"
                            "\x00\x00\x00\x05\x70\x61\x79\x6c\x6f\x61\x64\x00\x78\x00\x00\x00"
                            "\x00\x63\x3d\x62\x69\x77\x73\x2c\x72\x3d\x4a\x53\x79\x52\x48\x44"
                            "\x37\x73\x63\x39\x52\x67\x44\x43\x44\x7a\x4a\x4a\x4e\x56\x64\x6b"
                            "\x41\x32\x47\x6c\x53\x65\x4d\x4a\x50\x56\x35\x6e\x39\x35\x70\x30"
                            "\x77\x64\x5a\x7a\x78\x6a\x50\x74\x37\x7a\x79\x4c\x45\x4e\x66\x31"
                            "\x54\x6f\x38\x68\x59\x54\x62\x4b\x45\x51\x2c\x70\x3d\x68\x38\x2b"
                            "\x4b\x52\x48\x78\x46\x48\x6b\x72\x76\x43\x33\x74\x36\x43\x71\x36"
                            "\x4b\x56\x4c\x41\x74\x34\x6d\x6c\x42\x50\x2f\x56\x36\x4a\x72\x74"
                            "\x54\x4c\x4d\x4b\x76\x57\x2f\x34\x3d\x10\x63\x6f\x6e\x76\x65\x72"
                            "\x73\x61\x74\x69\x6f\x6e\x49\x64\x00\x01\x00\x00\x00\x02\x24\x64"
                            "\x62\x00\x05\x00\x00\x00\x74\x68\x6f\x72\x00\x00"s; //"\xbd\x34\x27\xd8"s;

    Op_MsgHeader::messageIdSetForTest(0x2);
    AuthCont        authCont(1, "thor", "c=biws,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,p=h8+KRHxFHkrvC3t6Cq6KVLAt4mlBP/V6JrtTLMKvW/4="s);
    Op_MsgAuthCont  authContMessage(OP_MsgFlag::empty, authCont);

    std::stringstream stream;
    stream << authContMessage;

    EXPECT_EQ(expected, stream.str());
}

TEST(HandShakeTest, CreateSASLSecondMessageReplicateGeneration)
{
    ThorsAnvil::Crypto::ScramClientSha256   client("loki"s, [](){return "JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV";});
    ThorsAnvil::Crypto::ScramServerSha256   server( client.getFirstMessage(),
                                                    15000,
                                                    [](){return "5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ";},
                                                    [](ThorsAnvil::Crypto::DBInfoType type, std::string const&)
                                                    {
                                                        return type == ThorsAnvil::Crypto::DBInfoType::Password
                                                            ? "underworldSA0"
                                                            : "4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==";
                                                    });
    std::string proof = client.getProofMessage("underworldSA0", server.getFirstMessage());

    EXPECT_EQ("c=biws,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,p=h8+KRHxFHkrvC3t6Cq6KVLAt4mlBP/V6JrtTLMKvW/4="s, proof);
}

TEST(HandShakeTest, ReadSASLSecondMessageResponse)
{
#if 0
RAW
81 00 00 00 9e 11 00 00   ?...?...........
02 00 00 00 dd 07 00 00 01 00 00 00 00 68 00 00   .............h..
00 10 63 6f 6e 76 65 72 73 61 74 69 6f 6e 49 64   ..conversationId
00 01 00 00 00 08 64 6f 6e 65 00 00 05 70 61 79   ......done...pay
6c 6f 61 64 00 2e 00 00 00 00 76 3d 46 7a 45 2f   load......v=FzE/
76 34 66 35 44 63 65 39 38 47 6f 57 35 77 66 4b   v4f5Dce98GoW5wfK
52 72 51 61 42 51 73 6a 6a 33 48 39 37 36 6f 70   RrQaBQsjj3H976op
58 78 34 49 66 67 49 3d 01 6f 6b 00 00 00 00 00   Xx4IfgI=.ok.....
00 00 f0 3f 00 71 02 ad 2f                        ...?.q../

Hand split raw data
81 00 00 00         // Size
9e 11 00 00         // Message Id
02 00 00 00         // Response To
dd 07 00 00         // OP_CODE: 2013 MSG
MSG
01 00 00 00         // Flags
00                  // Kind0
BSON
68 00 00 00
    10      conversationId              01 00 00 00
    08      done                        00
    05      payload                     2e 00 00 00     00      v=FzE/v4f5Dce98GoW5wfKRrQaBQsjj3H976opXx4IfgI=
    01      ok                          00 00 00 00 00 00 f0 3f
00
71 02 ad 2f         // Checksum
#endif
    std::string input = "\x81\x00\x00\x00\x9e\x11\x00\x00"
                        "\x02\x00\x00\x00\xdd\x07\x00\x00\x01\x00\x00\x00\x00\x68\x00\x00"
                        "\x00\x10\x63\x6f\x6e\x76\x65\x72\x73\x61\x74\x69\x6f\x6e\x49\x64"
                        "\x00\x01\x00\x00\x00\x08\x64\x6f\x6e\x65\x00\x00\x05\x70\x61\x79"
                        "\x6c\x6f\x61\x64\x00\x2e\x00\x00\x00\x00\x76\x3d\x46\x7a\x45\x2f"
                        "\x76\x34\x66\x35\x44\x63\x65\x39\x38\x47\x6f\x57\x35\x77\x66\x4b"
                        "\x52\x72\x51\x61\x42\x51\x73\x6a\x6a\x33\x48\x39\x37\x36\x6f\x70"
                        "\x58\x78\x34\x49\x66\x67\x49\x3d\x01\x6f\x6b\x00\x00\x00\x00\x00"
                        "\x00\x00\xf0\x3f\x00\x71\x02\xad\x2f"s;

    Op_MsgAuthReply     authReplyMessage;

    std::stringstream stream(input);
    stream >> authReplyMessage;

    AuthReply const& authReply = authReplyMessage.getAction();
    EXPECT_EQ(authReply.conversationId,   1);
    EXPECT_EQ(authReply.done,             false);
    EXPECT_EQ(authReply.payload.data,     "v=FzE/v4f5Dce98GoW5wfKRrQaBQsjj3H976opXx4IfgI="s);
    EXPECT_EQ(authReply.ok,               1.0);
}

TEST(HandShakeTest, ReadSASLSecondMessageResponseReplicateGeneration)
{
    ThorsAnvil::Crypto::ScramClientSha256   client("loki"s, [](){return "JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV";});
    ThorsAnvil::Crypto::ScramServerSha256   server( client.getFirstMessage(),
                                                    15000,
                                                    [](){return "5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ";},
                                                    [](ThorsAnvil::Crypto::DBInfoType type, std::string const&)
                                                    {
                                                        return type == ThorsAnvil::Crypto::DBInfoType::Password
                                                            ? "underworldSA0"
                                                            : "4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==";
                                                    });
    std::string proof = client.getProofMessage("underworldSA0", server.getFirstMessage());

    std::string valid = server.getProofMessage(proof);

    EXPECT_EQ("v=FzE/v4f5Dce98GoW5wfKRrQaBQsjj3H976opXx4IfgI="s, valid);
}

TEST(HandShakeTest, CreateSASLThirdMessage)
{
#if 0
RAW
60 00 00 00 03 00 00 00   ?...?...`.......
00 00 00 00 dd 07 00 00 01 00 00 00 00 47 00 00   .............G..
00 10 73 61 73 6c 43 6f 6e 74 69 6e 75 65 00 01   ..saslContinue..
00 00 00 05 70 61 79 6c 6f 61 64 00 00 00 00 00   ....payload.....
00 10 63 6f 6e 76 65 72 73 61 74 69 6f 6e 49 64   ..conversationId
00 01 00 00 00 02 24 64 62 00 05 00 00 00 74 68   ......$db.....th
6f 72 00 00 3d 9c 10 02                           or..=...

Hand Split Raw Message
60 00 00 00         // Size
03 00 00 00         // Message Id
00 00 00 00         // Reply To
dd 07 00 00         // OP_Code: 2013: Op_Msg
MSG
01 00 00 00         // Flag
00                  // Kind0
BSON
47 00 00 00         // Size
    10      saslContinue    01 00 00 00
    05      payload         00 00 00 00     00
    10      conversationId  01 00 00 00
    02      $db             05 00 00 00     thor
00
3d 9c 10 02         // Checksum
#endif
                            // have not implemented checksum so slightly modified.
    std::string expected =  /* Change Size "\x60" remove 4*/ "\x5c" "\x00\x00\x00\x03\x00\x00\x00"
                            "\x00\x00\x00\x00\xdd\x07\x00\x00" /* Remove Flag for checksum "\x01"*/ "\x00" "\x00\x00\x00\x00\x47\x00\x00"
                            "\x00\x10\x73\x61\x73\x6c\x43\x6f\x6e\x74\x69\x6e\x75\x65\x00\x01"
                            "\x00\x00\x00\x05\x70\x61\x79\x6c\x6f\x61\x64\x00\x00\x00\x00\x00"
                            "\x00\x10\x63\x6f\x6e\x76\x65\x72\x73\x61\x74\x69\x6f\x6e\x49\x64"
                            "\x00\x01\x00\x00\x00\x02\x24\x64\x62\x00\x05\x00\x00\x00\x74\x68"
                            "\x6f\x72\x00\x00"s;//\x3d\x9c\x10\x02"s;

    Op_MsgHeader::messageIdSetForTest(0x3);
    AuthCont        authCont(1, "thor"s, ""s);
    Op_MsgAuthCont  authContMessage(OP_MsgFlag::empty, authCont);

    std::stringstream stream;
    stream << authContMessage;

    EXPECT_EQ(expected, stream.str());
}

TEST(HandShakeTest, ReadSASLThirdMessageResponse)
{
#if 0
53 00 00 00 9f 11 00 00   ?...?...S.......
03 00 00 00 dd 07 00 00 01 00 00 00 00 3a 00 00   .............:..
00 10 63 6f 6e 76 65 72 73 61 74 69 6f 6e 49 64   ..conversationId
00 01 00 00 00 08 64 6f 6e 65 00 01 05 70 61 79   ......done...pay
6c 6f 61 64 00 00 00 00 00 00 01 6f 6b 00 00 00   load.......ok...
00 00 00 00 f0 3f 00 34 67 da 52                  .....?.4g.R

53 00 00 00         // Size
9f 11 00 00         // Message Id
03 00 00 00         // Response To
dd 07 00 00         // Op_CODE: 2013: Op_Msg
MSG
01 00 00 00         // Flags
00                  // Kind0
BSON
3a 00 00 00         // Size
    10      conversationId          01 00 00 00
    08      done                    01
    05      payload                 00 00 00 00     00
    01      ok                      00 00 00 00 00 00 f0 3f
00
34 67 da 52         // Checksum
#endif

    std::string input = "\x53\x00\x00\x00\x9f\x11\x00\x00"
                        "\x03\x00\x00\x00\xdd\x07\x00\x00\x01\x00\x00\x00\x00\x3a\x00\x00"
                        "\x00\x10\x63\x6f\x6e\x76\x65\x72\x73\x61\x74\x69\x6f\x6e\x49\x64"
                        "\x00\x01\x00\x00\x00\x08\x64\x6f\x6e\x65\x00\x01\x05\x70\x61\x79"
                        "\x6c\x6f\x61\x64\x00\x00\x00\x00\x00\x00\x01\x6f\x6b\x00\x00\x00"
                        "\x00\x00\x00\x00\xf0\x3f\x00\x34\x67\xda\x52"s;

    Op_MsgAuthReply     authReplyMessage;

    std::stringstream stream(input);
    stream >> authReplyMessage;

    AuthReply const& authReply = authReplyMessage.getAction();
    EXPECT_EQ(authReply.conversationId,   1);
    EXPECT_EQ(authReply.done,             true);
    EXPECT_EQ(authReply.payload.data,     "");
    EXPECT_EQ(authReply.ok,               1.0);
}

#if 0
21: To Server
3d 00 00 00 04 00 00 00   ?...?...=.......
00 00 00 00 dd 07 00 00 01 00 00 00 00 24 00 00   .............$..
00 10 77 68 61 74 73 6d 79 75 72 69 00 01 00 00   ..whatsmyuri....
00 02 24 64 62 00 06 00 00 00 61 64 6d 69 6e 00   ..$db.....admin.
00 6c 09 be a1                                    .l...


3d 00 00 00         Size
04 00 00 00         MessageId
00 00 00 00         Response To
dd 07 00 00         Op_CODE: 2013 Op_Msg
MSG
01 00 00 00         Flags
00
BSON
24 00 00 00         Size
    10  whatsmyuri  01 00 00 00
    02  $db         06 00 00 00     admin
00
6c 09 be a1         Checksum

-------

23: From Server
43 00 00 00 a0 11 00 00   ?...?...C.......
04 00 00 00 dd 07 00 00 01 00 00 00 00 2a 00 00   .............*..
00 02 79 6f 75 00 10 00 00 00 31 32 37 2e 30 2e   ..you.....127.0.
30 2e 31 3a 35 37 34 35 38 00 01 6f 6b 00 00 00   0.1:57458..ok...
00 00 00 00 f0 3f 00 e4 e1 7c 19                  .....?...|.

43 00 00 00         Size
a0 11 00 00         MessageId
04 00 00 00         Response To
dd 07 00 00         Op_CODE 2013 Op_Msg
Msg
01 00 00 00         Flags
00
BSON
2a 00 00 00
    02  you     10 00 00 00                 127.0.0.1:57458
    01  ok      00 00 00 00 00 00 f0 3f
00
e4 e1 7c 19         Checksum

-------

25: To Server
40 00 00 00 05 00 00 00   ?...?...@.......
00 00 00 00 dd 07 00 00 01 00 00 00 00 27 00 00   .............X..
00 01 62 75 69 6c 64 69 6e 66 6f 00 00 00 00 00   ..buildinfo.....
00 00 f0 3f 02 24 64 62 00 06 00 00 00 61 64 6d   ...?.$db.....adm
69 6e 00 00 c2 ed 7a 97                           in....z.

40 00 00 00         Size
05 00 00 00         Message Id
00 00 00 00         Response To
dd 07 00 00         Op_CODE 2013 Op_Msg
Msg
01 00 00 00         Flags
00
BSON
27 00 00 00         Size
    01  buildinfo       00 00 00 00 00 00 f0 3f
    02  $db             06 00 00 00     admin
00
c2 ed 7a 97         Checksum

------

27: From Server
30 0a 00 00 a1 11 00 00   ?...?...0.......
05 00 00 00 dd 07 00 00 01 00 00 00 00 17 0a 00   ................
00 02 76 65 72 73 69 6f 6e 00 06 00 00 00 34 2e   ..version.....4.
34 2e 30 00 02 67 69 74 56 65 72 73 69 6f 6e 00   4.0..gitVersion.
29 00 00 00 35 36 33 34 38 37 65 31 30 30 63 34   )...563487e100c4
32 31 35 65 32 64 63 65 39 38 64 30 61 66 32 61   215e2dce98d0af2a
36 61 35 61 32 64 36 37 63 35 63 66 00 04 6d 6f   6a5a2d67c5cf..mo
64 75 6c 65 73 00 05 00 00 00 00 02 61 6c 6c 6f   dules.......allo
63 61 74 6f 72 00 07 00 00 00 73 79 73 74 65 6d   cator.....system
00 02 6a 61 76 61 73 63 72 69 70 74 45 6e 67 69   ..javascriptEngi
6e 65 00 06 00 00 00 6d 6f 7a 6a 73 00 02 73 79   ne.....mozjs..sy
73 49 6e 66 6f 00 0b 00 00 00 64 65 70 72 65 63   sInfo.....deprec
61 74 65 64 00 04 76 65 72 73 69 6f 6e 41 72 72   ated..versionArr
61 79 00 21 00 00 00 10 30 00 04 00 00 00 10 31   ay.!....0......1
00 04 00 00 00 10 32 00 00 00 00 00 10 33 00 00   ......2......3..
00 00 00 00 03 6f 70 65 6e 73 73 6c 00 29 00 00   .....openssl.)..
00 02 72 75 6e 6e 69 6e 67 00 17 00 00 00 41 70   ..running.....Ap
70 6c 65 20 53 65 63 75 72 65 20 54 72 61 6e 73   ple Secure Trans
70 6f 72 74 00 00 03 62 75 69 6c 64 45 6e 76 69   port...buildEnvi
72 6f 6e 6d 65 6e 74 00 6a 08 00 00 02 64 69 73   ronment.j....dis
74 6d 6f 64 00 01 00 00 00 00 02 64 69 73 74 61   tmod.......dista
72 63 68 00 07 00 00 00 78 38 36 5f 36 34 00 02   rch.....x86_64..
63 63 00 92 00 00 00 2f 41 70 70 6c 69 63 61 74   cc...../Applicat
69 6f 6e 73 2f 58 63 6f 64 65 31 30 2e 32 2e 30   ions/Xcode10.2.0
2e 61 70 70 2f 43 6f 6e 74 65 6e 74 73 2f 44 65   .app/Contents/De
76 65 6c 6f 70 65 72 2f 54 6f 6f 6c 63 68 61 69   veloper/Toolchai
6e 73 2f 58 63 6f 64 65 44 65 66 61 75 6c 74 2e   ns/XcodeDefault.
78 63 74 6f 6f 6c 63 68 61 69 6e 2f 75 73 72 2f   xctoolchain/usr/
62 69 6e 2f 63 6c 61 6e 67 3a 20 41 70 70 6c 65   bin/clang: Apple
20 4c 4c 56 4d 20 76 65 72 73 69 6f 6e 20 31 30    LLVM version 10
2e 30 2e 31 20 28 63 6c 61 6e 67 2d 31 30 30 31   .0.1 (clang-1001
2e 30 2e 34 36 2e 33 29 00 02 63 63 66 6c 61 67   .0.46.3)..ccflag
73 00 46 03 00 00 2d 69 73 79 73 72 6f 6f 74 20   s.F...-isysroot
2f 41 70 70 6c 69 63 61 74 69 6f 6e 73 2f 58 63   /Applications/Xc
6f 64 65 31 30 2e 32 2e 30 2e 61 70 70 2f 43 6f   ode10.2.0.app/Co
6e 74 65 6e 74 73 2f 44 65 76 65 6c 6f 70 65 72   ntents/Developer
2f 50 6c 61 74 66 6f 72 6d 73 2f 4d 61 63 4f 53   /Platforms/MacOS
58 2e 70 6c 61 74 66 6f 72 6d 2f 44 65 76 65 6c   X.platform/Devel
6f 70 65 72 2f 53 44 4b 73 2f 4d 61 63 4f 53 58   oper/SDKs/MacOSX
31 30 2e 31 34 2e 73 64 6b 20 2d 6d 6d 61 63 6f   10.14.sdk -mmaco
73 78 2d 76 65 72 73 69 6f 6e 2d 6d 69 6e 3d 31   sx-version-min=1
30 2e 31 33 20 2d 74 61 72 67 65 74 20 64 61 72   0.13 -target dar
77 69 6e 31 37 2e 30 2e 30 20 2d 61 72 63 68 20   win17.0.0 -arch
78 38 36 5f 36 34 20 2d 66 6e 6f 2d 6f 6d 69 74   x86_64 -fno-omit
2d 66 72 61 6d 65 2d 70 6f 69 6e 74 65 72 20 2d   -frame-pointer -
66 6e 6f 2d 73 74 72 69 63 74 2d 61 6c 69 61 73   fno-strict-alias
69 6e 67 20 2d 66 61 73 79 6e 63 68 72 6f 6e 6f   ing -fasynchrono
75 73 2d 75 6e 77 69 6e 64 2d 74 61 62 6c 65 73   us-unwind-tables
20 2d 67 67 64 62 20 2d 70 74 68 72 65 61 64 20    -ggdb -pthread
2d 57 61 6c 6c 20 2d 57 73 69 67 6e 2d 63 6f 6d   -Wall -Wsign-com
70 61 72 65 20 2d 57 6e 6f 2d 75 6e 6b 6e 6f 77   pare -Wno-unknow
6e 2d 70 72 61 67 6d 61 73 20 2d 57 69 6e 76 61   n-pragmas -Winva
6c 69 64 2d 70 63 68 20 2d 57 65 72 72 6f 72 20   lid-pch -Werror
2d 4f 32 20 2d 57 6e 6f 2d 75 6e 75 73 65 64 2d   -O2 -Wno-unused-
6c 6f 63 61 6c 2d 74 79 70 65 64 65 66 73 20 2d   local-typedefs -
57 6e 6f 2d 75 6e 75 73 65 64 2d 66 75 6e 63 74   Wno-unused-funct
69 6f 6e 20 2d 57 6e 6f 2d 75 6e 75 73 65 64 2d   ion -Wno-unused-
70 72 69 76 61 74 65 2d 66 69 65 6c 64 20 2d 57   private-field -W
6e 6f 2d 64 65 70 72 65 63 61 74 65 64 2d 64 65   no-deprecated-de
63 6c 61 72 61 74 69 6f 6e 73 20 2d 57 6e 6f 2d   clarations -Wno-
74 61 75 74 6f 6c 6f 67 69 63 61 6c 2d 63 6f 6e   tautological-con
73 74 61 6e 74 2d 6f 75 74 2d 6f 66 2d 72 61 6e   stant-out-of-ran
67 65 2d 63 6f 6d 70 61 72 65 20 2d 57 6e 6f 2d   ge-compare -Wno-
74 61 75 74 6f 6c 6f 67 69 63 61 6c 2d 63 6f 6e   tautological-con
73 74 61 6e 74 2d 63 6f 6d 70 61 72 65 20 2d 57   stant-compare -W
6e 6f 2d 74 61 75 74 6f 6c 6f 67 69 63 61 6c 2d   no-tautological-
75 6e 73 69 67 6e 65 64 2d 7a 65 72 6f 2d 63 6f   unsigned-zero-co
6d 70 61 72 65 20 2d 57 6e 6f 2d 74 61 75 74 6f   mpare -Wno-tauto
6c 6f 67 69 63 61 6c 2d 75 6e 73 69 67 6e 65 64   logical-unsigned
2d 65 6e 75 6d 2d 7a 65 72 6f 2d 63 6f 6d 70 61   -enum-zero-compa
72 65 20 2d 57 6e 6f 2d 75 6e 75 73 65 64 2d 63   re -Wno-unused-c
6f 6e 73 74 2d 76 61 72 69 61 62 6c 65 20 2d 57   onst-variable -W
6e 6f 2d 6d 69 73 73 69 6e 67 2d 62 72 61 63 65   no-missing-brace
73 20 2d 57 6e 6f 2d 69 6e 63 6f 6e 73 69 73 74   s -Wno-inconsist
65 6e 74 2d 6d 69 73 73 69 6e 67 2d 6f 76 65 72   ent-missing-over
72 69 64 65 20 2d 57 6e 6f 2d 70 6f 74 65 6e 74   ride -Wno-potent
69 61 6c 6c 79 2d 65 76 61 6c 75 61 74 65 64 2d   ially-evaluated-
65 78 70 72 65 73 73 69 6f 6e 20 2d 57 6e 6f 2d   expression -Wno-
75 6e 75 73 65 64 2d 6c 61 6d 62 64 61 2d 63 61   unused-lambda-ca
70 74 75 72 65 20 2d 57 6e 6f 2d 65 78 63 65 70   pture -Wno-excep
74 69 6f 6e 73 20 2d 57 75 6e 67 75 61 72 64 65   tions -Wunguarde
64 2d 61 76 61 69 6c 61 62 69 6c 69 74 79 20 2d   d-availability -
66 73 74 61 63 6b 2d 70 72 6f 74 65 63 74 6f 72   fstack-protector
2d 73 74 72 6f 6e 67 20 2d 66 6e 6f 2d 62 75 69   -strong -fno-bui
6c 74 69 6e 2d 6d 65 6d 63 6d 70 00 02 63 78 78   ltin-memcmp..cxx
00 94 00 00 00 2f 41 70 70 6c 69 63 61 74 69 6f   ...../Applicatio
6e 73 2f 58 63 6f 64 65 31 30 2e 32 2e 30 2e 61   ns/Xcode10.2.0.a
70 70 2f 43 6f 6e 74 65 6e 74 73 2f 44 65 76 65   pp/Contents/Deve
6c 6f 70 65 72 2f 54 6f 6f 6c 63 68 61 69 6e 73   loper/Toolchains
2f 58 63 6f 64 65 44 65 66 61 75 6c 74 2e 78 63   /XcodeDefault.xc
74 6f 6f 6c 63 68 61 69 6e 2f 75 73 72 2f 62 69   toolchain/usr/bi
6e 2f 63 6c 61 6e 67 2b 2b 3a 20 41 70 70 6c 65   n/clang++: Apple
20 4c 4c 56 4d 20 76 65 72 73 69 6f 6e 20 31 30    LLVM version 10
2e 30 2e 31 20 28 63 6c 61 6e 67 2d 31 30 30 31   .0.1 (clang-1001
2e 30 2e 34 36 2e 33 29 00 02 63 78 78 66 6c 61   .0.46.3)..cxxfla
67 73 00 df 00 00 00 2d 57 6f 76 65 72 6c 6f 61   gs.....-Woverloa
64 65 64 2d 76 69 72 74 75 61 6c 20 2d 57 65 72   ded-virtual -Wer
72 6f 72 3d 75 6e 75 73 65 64 2d 72 65 73 75 6c   ror=unused-resul
74 20 2d 57 70 65 73 73 69 6d 69 7a 69 6e 67 2d   t -Wpessimizing-
6d 6f 76 65 20 2d 57 72 65 64 75 6e 64 61 6e 74   move -Wredundant
2d 6d 6f 76 65 20 2d 57 6e 6f 2d 75 6e 64 65 66   -move -Wno-undef
69 6e 65 64 2d 76 61 72 2d 74 65 6d 70 6c 61 74   ined-var-templat
65 20 2d 57 6e 6f 2d 69 6e 73 74 61 6e 74 69 61   e -Wno-instantia
74 69 6f 6e 2d 61 66 74 65 72 2d 73 70 65 63 69   tion-after-speci
61 6c 69 7a 61 74 69 6f 6e 20 2d 66 73 69 7a 65   alization -fsize
64 2d 64 65 61 6c 6c 6f 63 61 74 69 6f 6e 20 2d   d-deallocation -
57 75 6e 75 73 65 64 2d 65 78 63 65 70 74 69 6f   Wunused-exceptio
6e 2d 70 61 72 61 6d 65 74 65 72 20 2d 73 74 64   n-parameter -std
6c 69 62 3d 6c 69 62 63 2b 2b 20 2d 73 74 64 3d   lib=libc++ -std=
63 2b 2b 31 37 00 02 6c 69 6e 6b 66 6c 61 67 73   c++17..linkflags
00 4a 01 00 00 2d 57 6c 2c 2d 73 79 73 6c 69 62   .J...-Wl,-syslib
72 6f 6f 74 2c 2f 41 70 70 6c 69 63 61 74 69 6f   root,/Applicatio
6e 73 2f 58 63 6f 64 65 31 30 2e 32 2e 30 2e 61   ns/Xcode10.2.0.a
70 70 2f 43 6f 6e 74 65 6e 74 73 2f 44 65 76 65   pp/Contents/Deve
6c 6f 70 65 72 2f 50 6c 61 74 66 6f 72 6d 73 2f   loper/Platforms/
4d 61 63 4f 53 58 2e 70 6c 61 74 66 6f 72 6d 2f   MacOSX.platform/
44 65 76 65 6c 6f 70 65 72 2f 53 44 4b 73 2f 4d   Developer/SDKs/M
61 63 4f 53 58 31 30 2e 31 34 2e 73 64 6b 20 2d   acOSX10.14.sdk -
6d 6d 61 63 6f 73 78 2d 76 65 72 73 69 6f 6e 2d   mmacosx-version-
6d 69 6e 3d 31 30 2e 31 33 20 2d 74 61 72 67 65   min=10.13 -targe
74 20 64 61 72 77 69 6e 31 37 2e 30 2e 30 20 2d   t darwin17.0.0 -
61 72 63 68 20 78 38 36 5f 36 34 20 2d 57 6c 2c   arch x86_64 -Wl,
2d 62 69 6e 64 5f 61 74 5f 6c 6f 61 64 20 2d 57   -bind_at_load -W
6c 2c 2d 66 61 74 61 6c 5f 77 61 72 6e 69 6e 67   l,-fatal_warning
73 20 2d 66 73 74 61 63 6b 2d 70 72 6f 74 65 63   s -fstack-protec
74 6f 72 2d 73 74 72 6f 6e 67 20 2d 73 74 64 6c   tor-strong -stdl
69 62 3d 6c 69 62 63 2b 2b 20 2d 57 6c 2c 2d 72   ib=libc++ -Wl,-r
70 61 74 68 2c 40 6c 6f 61 64 65 72 5f 70 61 74   path,@loader_pat
68 2f 2e 2e 2f 6c 69 62 20 2d 57 6c 2c 2d 72 70   h/../lib -Wl,-rp
61 74 68 2c 40 6c 6f 61 64 65 72 5f 70 61 74 68   ath,@loader_path
2f 2e 2e 2f 46 72 61 6d 65 77 6f 72 6b 73 00 02   /../Frameworks..
74 61 72 67 65 74 5f 61 72 63 68 00 07 00 00 00   target_arch.....
78 38 36 5f 36 34 00 02 74 61 72 67 65 74 5f 6f   x86_64..target_o
73 00 06 00 00 00 6d 61 63 4f 53 00 02 63 70 70   s.....macOS..cpp
64 65 66 69 6e 65 73 00 35 01 00 00 53 41 46 45   defines.5...SAFE
49 4e 54 5f 55 53 45 5f 49 4e 54 52 49 4e 53 49   INT_USE_INTRINSI
43 53 20 30 20 50 43 52 45 5f 53 54 41 54 49 43   CS 0 PCRE_STATIC
20 4e 44 45 42 55 47 20 42 4f 4f 53 54 5f 54 48    NDEBUG BOOST_TH
52 45 41 44 5f 56 45 52 53 49 4f 4e 20 35 20 42   READ_VERSION 5 B
4f 4f 53 54 5f 54 48 52 45 41 44 5f 55 53 45 53   OOST_THREAD_USES
5f 44 41 54 45 54 49 4d 45 20 42 4f 4f 53 54 5f   _DATETIME BOOST_
53 59 53 54 45 4d 5f 4e 4f 5f 44 45 50 52 45 43   SYSTEM_NO_DEPREC
41 54 45 44 20 42 4f 4f 53 54 5f 4d 41 54 48 5f   ATED BOOST_MATH_
4e 4f 5f 4c 4f 4e 47 5f 44 4f 55 42 4c 45 5f 4d   NO_LONG_DOUBLE_M
41 54 48 5f 46 55 4e 43 54 49 4f 4e 53 20 42 4f   ATH_FUNCTIONS BO
4f 53 54 5f 45 4e 41 42 4c 45 5f 41 53 53 45 52   OST_ENABLE_ASSER
54 5f 44 45 42 55 47 5f 48 41 4e 44 4c 45 52 20   T_DEBUG_HANDLER
42 4f 4f 53 54 5f 4c 4f 47 5f 4e 4f 5f 53 48 4f   BOOST_LOG_NO_SHO
52 54 48 41 4e 44 5f 4e 41 4d 45 53 20 42 4f 4f   RTHAND_NAMES BOO
53 54 5f 4c 4f 47 5f 55 53 45 5f 4e 41 54 49 56   ST_LOG_USE_NATIV
45 5f 53 59 53 4c 4f 47 20 42 4f 4f 53 54 5f 4c   E_SYSLOG BOOST_L
4f 47 5f 57 49 54 48 4f 55 54 5f 54 48 52 45 41   OG_WITHOUT_THREA
44 5f 41 54 54 52 20 41 42 53 4c 5f 46 4f 52 43   D_ATTR ABSL_FORC
45 5f 41 4c 49 47 4e 45 44 5f 41 43 43 45 53 53   E_ALIGNED_ACCESS
00 00 10 62 69 74 73 00 40 00 00 00 08 64 65 62   ...bits.@....deb
75 67 00 00 10 6d 61 78 42 73 6f 6e 4f 62 6a 65   ug...maxBsonObje
63 74 53 69 7a 65 00 00 00 00 01 04 73 74 6f 72   ctSize......stor
61 67 65 45 6e 67 69 6e 65 73 00 4c 00 00 00 02   ageEngines.L....
30 00 07 00 00 00 62 69 67 67 69 65 00 02 31 00   0.....biggie..1.
08 00 00 00 64 65 76 6e 75 6c 6c 00 02 32 00 11   ....devnull..2..
00 00 00 65 70 68 65 6d 65 72 61 6c 46 6f 72 54   ...ephemeralForT
65 73 74 00 02 33 00 0b 00 00 00 77 69 72 65 64   est..3.....wired
54 69 67 65 72 00 00 01 6f 6b 00 00 00 00 00 00   Tiger...ok......
00 f0 3f 00 21 51 b3 eb                           ..?.!Q..

30 0a 00 00         Size
a1 11 00 00         MessageId
05 00 00 00         Response To
dd 07 00 00         Op_CODE 2013 Op_Msg
Msg
01 00 00 00         Flags
00
BSON
17 0a 00 00         Size
    02  version     06 00 00 00     4.4.0
    02  gitVersion  29 00 00 00     563487e100c4215e2dce98d0af2a6a5a2d67c5cf
    04  modules
    05 00 00 00
    00
    02  allocator           07 00 00 00     system
    02  javascriptEngine    06 00 00 00     mozjs
    02  sysInfo             0b 00 00 00     deprecated
    04  versionArray        21 00 00 00
        10  0   04 00 00 00
        10  1   04 00 00 00
        10  2   00 00 00 00
        10  3   00 00 00 00
    00
    03  openssl             29 00 00 00
        02 running          17 00 00 00     Apple Secure Transport
    00
    03  buildEnvironment
    6a 08 00 00
        02  distmod         01 00 00 00     "" /* Empty*/
        02  distarch        07 00 00 00     x86_64
        02  cc              92 00 00 00     /Applications/Xcode10.2.0.app/Contents/Developer/Toolchains/XcodeDefault. xctoolchain/usr/bin/clang: AppleLLVM version 10.0.1 (clang-1001.0.46.3)
        02  ccflags         46 03 00 00     -isysroot/Applications/Xcode10.2.0.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk -mmacosx-version-min=10.13 -target darwin17.0.0 -archx86_64 -fno-omit-frame-pointer -fno-strict-aliasing -fasynchronous-unwind-tables-ggdb -pthread-Wall -Wsign-compare -Wno-unknown-pragmas -Winvalid-pch -Werror-O2 -Wno-unused-local-typedefs -Wno-unused-function -Wno-unused-private-field -Wno-deprecated-declarations -Wno-tautological-constant-out-of-range-compare -Wno-tautological-constant-compare -Wno-tautological-unsigned-zero-compare -Wno-tautological-unsigned-enum-zero-compare -Wno-unused-const-variable -Wno-missing-braces -Wno-inconsistent-missing-override -Wno-potentially-evaluated-expression -Wno-unused-lambda-capture -Wno-exceptions -Wunguarded-availability -fstack-protector-strong -fno-builtin-memcmp
        02  cxx             94 00 00 00     /Applications/Xcode10.2.0.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++: AppleLLVM version 10.0.1 (clang-1001.0.46.3)
        02  cxxflags        df 00 00 00     -Woverloaded-virtual -Werror=unused-result -Wpessimizing-move -Wredundant-move -Wno-undefined-var-template -Wno-instantiation-after-specialization -fsized-deallocation -Wunused-exception-parameter -stdlib=libc++ -std=c++17
        02  linkflags       4a 01 00 00     -Wl,-syslibroot,/Applications/Xcode10.2.0.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk -mmacosx-version-min=10.13 -target darwin17.0.0 -arch x86_64 -Wl,-bind_at_load -Wl,-fatal_warnings -fstack-protector-strong -stdlib=libc++ -Wl,-rpath,@loader_path/../lib -Wl,-rpath,@loader_path/../Frameworks
        02  target_arch     07 00 00 00     x86_64
        02  target_os       06 00 00 00     macOS
        02  cppdefines      35 01 00 00     SAFEINT_USE_INTRINSICS 0 PCRE_STATICNDEBUG BOOST_THREAD_VERSION 5 BOOST_THREAD_USES_DATETIME BOOST_SYSTEM_NO_DEPRECATED BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS BOOST_ENABLE_ASSERT_DEBUG_HANDLERBOOST_LOG_NO_SHORTHAND_NAMES BOOST_LOG_USE_NATIVE_SYSLOG BOOST_LOG_WITHOUT_THREAD_ATTR ABSL_FORCE_ALIGNED_ACCESS
    00
    10  bits                40 00 00 00
    08  debug               00
    10  maxBsonObjectSize   00 00 00 01
    04  storageEngines
    4c 00 00 00
        02  0   07 00 00 00     biggie
        02  1   08 00 00 00     devnull
        02  2   11 00 00 00     ephemeralForTest
        02  3   0b 00 00 00     wiredTiger
    00
    01  ok                  00 00 00 00 00 00 f0 3f
00
21 51 b3 eb             Checksum

-------

29: To Server
6d 00 00 00 06 00 00 00   ?...?...m.......
00 00 00 00 dd 07 00 00 01 00 00 00 00 54 00 00   .............T..
00 02 67 65 74 4c 6f 67 00 10 00 00 00 73 74 61   ..getLog.....sta
72 74 75 70 57 61 72 6e 69 6e 67 73 00 03 6c 73   rtupWarnings..ls
69 64 00 1e 00 00 00 05 69 64 00 10 00 00 00 04   id......id......
78 31 35 c9 5d 29 4f 8e 84 c6 71 66 c7 33 5d 89   x15.])O...qf.3].
00 02 24 64 62 00 06 00 00 00 61 64 6d 69 6e 00   ..$db.....admin.
00 c9 7e 7b 60                                    ..~{`


6d 00 00 00         Size
06 00 00 00         Message Id
00 00 00 00         Respond To
dd 07 00 00         Op_CODE 2013: Op_Msg
Msg
01 00 00 00         Flags
00
54 00 00 00
    02  getLog      10 00 00 00     startupWarnings
    03  ls
    1e 00 00 00
        05  id      10 00 00 00     04
                    78 31 35 c9 5d 29 4f 8e 84 c6 71 66 c7 33 5d 89
    00
    02  $db         06 00 00 00     admin
00
c9 7e 7b 60         Checksum

-----

31: From Server

-----

33: To Server
35: From Server

-----

37: To Server

74 00 00 00 08 00 00 00   ?...?...t.......
00 00 00 00 dd 07 00 00 01 00 00 00 00 5b 00 00   .............[..
00 01 69 73 4d 61 73 74 65 72 00 00 00 00 00 00   ..isMaster......
00 f0 3f 01 66 6f 72 53 68 65 6c 6c 00 00 00 00   ..?.forShell....
00 00 00 f0 3f 03 6c 73 69 64 00 1e 00 00 00 05   ....?.lsid......
69 64 00 10 00 00 00 04 78 31 35 c9 5d 29 4f 8e   id......x15.])O.
84 c6 71 66 c7 33 5d 89 00 02 24 64 62 00 05 00   ..qf.3]...$db...
00 00 74 68 6f 72 00 00 98 6d 89 b3               ..thor...m..


74 00 00 00         Size
08 00 00 00         Message Id
00 00 00 00         Remply To
dd 07 00 00         Op_CODE 2013 Op_Msg
MSg
01 00 00 00         Flags
00
BSON
5b 00 00 00         Size
    01  isMaster        00 00 00 00 00 00 f0 3f
    01  forShell        00 00 00 00 00 00 f0 3f
    03  lsid
    1e 00 00 00
        05      id      10 00 00 00     04
                        78 31 35 c9 5d 29 4f 8e 84 c6 71 66 c7 33 5d 89
    00
    02  $db             05 00 00 00     thors
00
98 6d 89 b3             // Checksum

-------

39: From Server

------

41: To Server
43: From Server

------

45: To Server
47: From Server

------

49: To Server
51: From Server

------

53: To Server
50 00 00 00         Size
0c 00 00 00         Message Id
00 00 00 00         Response To
dd 07 00 00         Op_CODE; 2013 Op_Msg
MSG
01 00 00 00         Flags
00
BSON
37 00 00 00         Size
    01  isMaster    00 00 00 00 00 00 f0 3f
    01  forShell    00 00 00 00 00 00 f0 3f
    02  $db         05 00 00 00         thor
00
00 13 d9 08         Checksum

-----

55: From Server

------

57: To Server
59: From Server

------

61: To Server
63: From Server

------
#endif

