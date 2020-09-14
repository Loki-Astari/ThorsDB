
#include <gtest/gtest.h>
#include "HandShake.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/CustomSerialization.h"
#include <sstream>

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

#ifdef   __APPLE__
    EXPECT_EQ(output, R"({"type":"Darwin","name":"MacOSX","architecture":"x86_64","version":""})");
#else
    std::cerr << output << "\n";
    EXPECT_EQ(output, "Please Add a test here");
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
    //EXPECT_EQ(output, R"({"application":{"name":"UnitTest"},"driver":{"name":"ThorsAnvil::Mongo::Driver","version":"v1.0"},"os":{},"platform":"ThorDB-Build"})");
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
34 00 02 76 65 72 73 69 6f 6e 00 07 00 00 00 31   4..version.....1
39 2e 36 2e 30 00 00 00 00                        9.6.0....

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
2a 01 00 00     // Size
    10      isMaster                01 00 00 00
    02      saslSupportedMechs      0a 00 00 00     thor.loki
    02      hostInfo                14 00 00 00     BatCave.local:27017
    03      client
    cb 00 00 00
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
        56 00 00 00
            02      type            07 00 00 00     Darwin
            02      name            09 00 00 00     Mac OS X
            02      architecture    07 00 00 00     x86_64
            02      version         07 00 00 00     19.6.0
        00
    00
00
#endif
    std::string expected =  "\x51\x01\x00\x00\x00\x00\x00\x00"
                            "\x00\x00\x00\x00\xd4\x07\x00\x00\x00\x00\x00\x00\x61\x64\x6d\x69"
                            "\x6e\x2e\x24\x63\x6d\x64\x00\x00\x00\x00\x00\x01\x00\x00\x00\x2a"
                            "\x01\x00\x00\x10\x69\x73\x4d\x61\x73\x74\x65\x72\x00\x01\x00\x00"
                            "\x00\x02\x73\x61\x73\x6c\x53\x75\x70\x70\x6f\x72\x74\x65\x64\x4d"
                            "\x65\x63\x68\x73\x00\x0a\x00\x00\x00\x74\x68\x6f\x72\x2e\x6c\x6f"
                            "\x6b\x69\x00\x02\x68\x6f\x73\x74\x49\x6e\x66\x6f\x00\x14\x00\x00"
                            "\x00\x42\x61\x74\x43\x61\x76\x65\x2e\x6c\x6f\x63\x61\x6c\x3a\x32"
                            "\x37\x30\x31\x37\x00\x03\x63\x6c\x69\x65\x6e\x74\x00\xcb\x00\x00"
                            "\x00\x03\x61\x70\x70\x6c\x69\x63\x61\x74\x69\x6f\x6e\x00\x1d\x00"
                            "\x00\x00\x02\x6e\x61\x6d\x65\x00\x0e\x00\x00\x00\x4d\x6f\x6e\x67"
                            "\x6f\x44\x42\x20\x53\x68\x65\x6c\x6c\x00\x00\x03\x64\x72\x69\x76"
                            "\x65\x72\x00\x3a\x00\x00\x00\x02\x6e\x61\x6d\x65\x00\x18\x00\x00"
                            "\x00\x4d\x6f\x6e\x67\x6f\x44\x42\x20\x49\x6e\x74\x65\x72\x6e\x61"
                            "\x6c\x20\x43\x6c\x69\x65\x6e\x74\x00\x02\x76\x65\x72\x73\x69\x6f"
                            "\x6e\x00\x06\x00\x00\x00\x34\x2e\x32\x2e\x30\x00\x00\x03\x6f\x73"
                            "\x00\x56\x00\x00\x00\x02\x74\x79\x70\x65\x00\x07\x00\x00\x00\x44"
                            "\x61\x72\x77\x69\x6e\x00\x02\x6e\x61\x6d\x65\x00\x09\x00\x00\x00"
                            "\x4d\x61\x63\x20\x4f\x53\x20\x58\x00\x02\x61\x72\x63\x68\x69\x74"
                            "\x65\x63\x74\x75\x72\x65\x00\x07\x00\x00\x00\x78\x38\x36\x5f\x36"
                            "\x34\x00\x02\x76\x65\x72\x73\x69\x6f\x6e\x00\x07\x00\x00\x00\x31"
                            "\x39\x2e\x36\x2e\x30\x00\x00\x00\x00"s;

    HandShake           handShake("MongoDB Shell", "MongoDB Internal Client", "4.2.0");
    Op_QueryHandShake   handShakeMessage(handShake);

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

    Op_ReplyHandShake   reply;
    stream >> reply;

    HandShakeReplyDoc const& doc = reply.getDocument(0);
    EXPECT_EQ(doc.ok,                   1.0);
    EXPECT_EQ(doc.ismaster,             1);
    EXPECT_EQ(doc.topologyVersion.processId, ThorsAnvil::Serialize::MongoUtility::ObjectID(0x5f48aa04, 0x2d1dfb1dd7L, 0x52e00));
    EXPECT_EQ(doc.topologyVersion.counter,   0x0L);
    EXPECT_EQ(doc.maxBsonObjectSize,    0x1000000);
    EXPECT_EQ(doc.maxMessageSizeBytes,  0x2dc6c00);
    EXPECT_EQ(doc.maxWriteBatchSize,    0x186a0);
    EXPECT_EQ(doc.localTime,            0x17476af112d);
    EXPECT_EQ(doc.logicalSessionTimeoutMinutes, 0x1e);
    EXPECT_EQ(doc.connectionId,         0x1e);
    EXPECT_EQ(doc.minWireVersion,       0);
    EXPECT_EQ(doc.maxWireVersion,       0x9);
    EXPECT_EQ(doc.readOnly,             false);
    ASSERT_EQ(doc.saslSupportedMechs.size(), 2);
    EXPECT_EQ(doc.saslSupportedMechs[0],"SCRAM-SHA-1");
    EXPECT_EQ(doc.saslSupportedMechs[1],"SCRAM-SHA-256");
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

    MsgHeader::messageIdSetForTest(0x1);
    AuthInit            authInit("thor"s, "SCRAM-SHA-256"s, "n,,n=loki,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV"s);
    Op_MsgAuthInit      authInitMessage(/*OP_MsgFlag::checksumPresent,*/authInit);

    std::stringstream   stream;
    stream << authInitMessage;

    EXPECT_EQ(expected, stream.str());
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
    std::stringstream stream(input);

    AuthInitReply           message;
    Op_MsgAuthInitReply     replyMessage(message);
    stream >> replyMessage;

    EXPECT_EQ(message.conversationId,   1);
    EXPECT_EQ(message.done,             false);
    EXPECT_EQ(message.payload.data,     "r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,s=4L0xAznFguS1rcZnj4WQnxLe1F570S+FQdKzOw==,i=15000"s);
    EXPECT_EQ(message.ok,               1.0);
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
62 00 05 00 00 00 74 68 6f 72 00 00 bd 34 27 d8   b.....thor...4'.

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

    MsgHeader::messageIdSetForTest(0x2);
    AuthCont        authCont(1, "thor", "c=biws,r=JSyRHD7sc9RgDCDzJJNVdkA2GlSeMJPV5n95p0wdZzxjPt7zyLENf1To8hYTbKEQ,p=h8+KRHxFHkrvC3t6Cq6KVLAt4mlBP/V6JrtTLMKvW/4="s);
    Op_MsgAuthCont  authContMessage(/*OP_MsgFlag::checksumPresent,*/authCont);

    std::stringstream stream;
    stream << authContMessage;

    EXPECT_EQ(expected, stream.str());
}



#if 0

81 00 00 00 9e 11 00 00   ?...?...........
02 00 00 00 dd 07 00 00 01 00 00 00 00 68 00 00   .............h..
00 10 63 6f 6e 76 65 72 73 61 74 69 6f 6e 49 64   ..conversationId
00 01 00 00 00 08 64 6f 6e 65 00 00 05 70 61 79   ......done...pay
6c 6f 61 64 00 2e 00 00 00 00 76 3d 46 7a 45 2f   load......v=FzE/
76 34 66 35 44 63 65 39 38 47 6f 57 35 77 66 4b   v4f5Dce98GoW5wfK
52 72 51 61 42 51 73 6a 6a 33 48 39 37 36 6f 70   RrQaBQsjj3H976op
58 78 34 49 66 67 49 3d 01 6f 6b 00 00 00 00 00   Xx4IfgI=.ok.....
00 00 f0 3f 00 71 02 ad 2f                        ...?.q../

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

----

60 00 00 00 03 00 00 00   ?...?...`.......
00 00 00 00 dd 07 00 00 01 00 00 00 00 47 00 00   .............G..
00 10 73 61 73 6c 43 6f 6e 74 69 6e 75 65 00 01   ..saslContinue..
00 00 00 05 70 61 79 6c 6f 61 64 00 00 00 00 00   ....payload.....
00 10 63 6f 6e 76 65 72 73 61 74 69 6f 6e 49 64   ..conversationId
00 01 00 00 00 02 24 64 62 00 05 00 00 00 74 68   ......$db.....th
6f 72 00 00 3d 9c 10 02                           or..=...

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

----

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

----

3d 00 00 00 04 00 00 00   ?...?...=.......
00 00 00 00 dd 07 00 00 01 00 00 00 00 24 00 00   .............$..
00 10 77 68 61 74 73 6d 79 75 72 69 00 01 00 00   ..whatsmyuri....
00 02 24 64 62 00 06 00 00 00 61 64 6d 69 6e 00   ..$db.....admin.
00 6c 09 be a1                                    .l...

3d 00 00 00         // Size
04 00 00 00         // Message Id
00 00 00 00         // Response To
dd 07 00 00         // OP_CODE: 2013: Op_MSG
MSG
01 00 00 00         // Flags
00                  // Kind0
BSON
24 00 00 00         // Size
    10      whatsmyuri              01 00 00 00
    02      $db                     06 00 00 00     admin.
00
6c 09 be a1         // Checksum

----

Summary:

Client                          Server
0 | 0 Query                                 119c | 0 Reply
1 | 0 Msg  saslStart 1                      119d | 1 Msg conversationId 1 done 0 ok 1
2 | 0 Msg  saslContinue 1 conversationId 1  119e | 2 Msg conversationId 1 done 0 ok 1
3 | 0 Msg  saslContinue 1 conversationId 1  119f | 3 Msg conversationId 1 done 1 ok 1

-----

4  0 



#endif

