
#include <gtest/gtest.h>
#include "Op_ReplyHandShake.h"
#include "ThorSerialize/CustomSerialization.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_ReplyHandshakeTest, HandShakeError)
{
    std::string input = "\xc0\x00\x00\x00\x47\x13\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00"
                        "\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                        "\x01\x00\x00\x00\x9c\x00\x00\x00\x01\x6f\x6b\x00\x00\x00\x00\x00"
                        "\x00\x00\x00\x00\x02\x65\x72\x72\x6d\x73\x67\x00\x5a\x00\x00\x00"
                        "\x54\x68\x65\x20\x27\x61\x70\x70\x6c\x69\x63\x61\x74\x69\x6f\x6e"
                        "\x27\x20\x66\x69\x65\x6c\x64\x20\x69\x73\x20\x72\x65\x71\x75\x69"
                        "\x72\x65\x64\x20\x74\x6f\x20\x62\x65\x20\x61\x20\x42\x53\x4f\x4e"
                        "\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x20\x69\x6e\x20\x74\x68\x65"
                        "\x20\x63\x6c\x69\x65\x6e\x74\x20\x6d\x65\x74\x61\x64\x61\x74\x61"
                        "\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x00\x10\x63\x6f\x64\x65\x00"
                        "\x0e\x00\x00\x00\x02\x63\x6f\x64\x65\x4e\x61\x6d\x65\x00\x0d\x00"
                        "\x00\x00\x54\x79\x70\x65\x4d\x69\x73\x6d\x61\x74\x63\x68\x00\x00"s;

    std::stringstream   inputStream(input);

    Op_ReplyHandShake   handShake;
    inputStream >> handShake;

    EXPECT_EQ(handShake.getDocument(0).ok,      0.0);
    EXPECT_EQ(handShake.getDocument(0).code,    14);
    EXPECT_EQ(handShake.getDocument(0).errmsg,  "The 'application' field is required to be a BSON document in the client metadata document");
    EXPECT_EQ(handShake.getDocument(0).codeName,"TypeMismatch");
}

TEST(Op_ReplyHandshakeTest, HandShakeErrorHumanReadable)
{
    std::string input = "\xc0\x00\x00\x00\x47\x13\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00"
                        "\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                        "\x01\x00\x00\x00\x9c\x00\x00\x00\x01\x6f\x6b\x00\x00\x00\x00\x00"
                        "\x00\x00\x00\x00\x02\x65\x72\x72\x6d\x73\x67\x00\x5a\x00\x00\x00"
                        "\x54\x68\x65\x20\x27\x61\x70\x70\x6c\x69\x63\x61\x74\x69\x6f\x6e"
                        "\x27\x20\x66\x69\x65\x6c\x64\x20\x69\x73\x20\x72\x65\x71\x75\x69"
                        "\x72\x65\x64\x20\x74\x6f\x20\x62\x65\x20\x61\x20\x42\x53\x4f\x4e"
                        "\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x20\x69\x6e\x20\x74\x68\x65"
                        "\x20\x63\x6c\x69\x65\x6e\x74\x20\x6d\x65\x74\x61\x64\x61\x74\x61"
                        "\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x00\x10\x63\x6f\x64\x65\x00"
                        "\x0e\x00\x00\x00\x02\x63\x6f\x64\x65\x4e\x61\x6d\x65\x00\x0d\x00"
                        "\x00\x00\x54\x79\x70\x65\x4d\x69\x73\x6d\x61\x74\x63\x68\x00\x00"s;

    std::stringstream   inputStream(input);

    Op_ReplyHandShake   handShake;
    inputStream >> handShake;

    std::stringstream   outputStream;
    outputStream << make_hr(handShake);
}

TEST(Op_ReplyHandshakeTest, HandShakeOK)
{
    std::string input=
            "\x80\x01\x00\x00"   // Len 384
            "\x77\x13\x00\x00"   // RequestId
            "\x01\x00\x00\x00"   // Response To
            "\x01\x00\x00\x00"   // OpCode OP_REPLY

            "\x08\x00\x00\x00"   // Flag
            "\x00\x00\x00\x00"   // Cursor P1
            "\x00\x00\x00\x00"   // Cursor P2
            "\x00\x00\x00\x00"   // Start
            "\x01\x00\x00\x00"   // Count

            //BSON Doc
            "\x5c\x01\x00\x00"
                "\x08"    "ismaster\0" "\x01"
                "\x03"    "topologyVersion\0"
                "\x2d\x00\x00\x00"
                    "\x07"    "processId\0"         "\x5f\x48\xaa\x04\x2d\x1d\xfb\x1d\xd7\x05\x2e\x00"
                    "\x12"    "counter\0"           "\x00\x00\x00\x00\x00\x00\x00\x00"
                "\x00"
                "\x10"    "maxBsonObjectSize\0"             "\x00\x00\x00\x01"
                "\x10"    "maxMessageSizeBytes\0"           "\x00\x6c\xdc\x02"
                "\x10"    "maxWriteBatchSize\0"             "\xa0\x86\x01\x00"
                "\x09"    "localTime\0"                     "\xa1\x29\x42\x7f\x74\x01\x00\x00"
                "\x10"    "logicalSessionTimeoutMinutes\0"  "\x1e\x00\x00\x00"
                "\x10"    "connectionId\0"                  "\x48\x00\x00\x00"
                "\x10"    "minWireVersion\0"                "\x00\x00\x00\x00"
                "\x10"    "maxWireVersion\0"                "\x09\x00\x00\x00"
                "\x08"    "readOnly\0"                      "\x00"
                "\x04"    "saslSupportedMechs\0"
                "\x2d\x00\x00\x00"
                    "\x02"    "0\0"     "\x0c\x00\x00\x00"   "SCRAM-SHA-1\0"
                    "\x02"    "1\0"     "\x0e\x00\x00\x00"   "SCRAM-SHA-256\0"
                "\x00"
                "\x01"    "ok\0"                            "\x00\x00\x00\x00\x00\x00\xf0\x3f"
            "\x00"s;
    std::stringstream   stream(input);

    Op_ReplyHandShake   handShake;
    stream >> handShake;

    std::cerr << make_hr(handShake);
    EXPECT_EQ(handShake.getDocument(0).ismaster,                    true);
    EXPECT_EQ(handShake.getDocument(0).topologyVersion.processId,   ThorsAnvil::Serialize::MongoUtility::ObjectID(0x5f48aa04, 0x2d1dfb1dd7L, 0x052e00));
    EXPECT_EQ(handShake.getDocument(0).topologyVersion.counter,     0);
    EXPECT_EQ(handShake.getDocument(0).maxBsonObjectSize,           0x1000000);
    EXPECT_EQ(handShake.getDocument(0).maxMessageSizeBytes,         0x2dc6c00);
    EXPECT_EQ(handShake.getDocument(0).maxWriteBatchSize,           0x186a0);
    EXPECT_EQ(handShake.getDocument(0).localTime,                   ThorsAnvil::Serialize::MongoUtility::UTCDateTime(0x1747f4229a1L));
    EXPECT_EQ(handShake.getDocument(0).logicalSessionTimeoutMinutes,0x1e);
    EXPECT_EQ(handShake.getDocument(0).connectionId,                0x48);
    EXPECT_EQ(handShake.getDocument(0).minWireVersion,              0x00);
    EXPECT_EQ(handShake.getDocument(0).maxWireVersion,              0x09);
    EXPECT_EQ(handShake.getDocument(0).readOnly,                    false);
    ASSERT_EQ(handShake.getDocument(0).saslSupportedMechs.size(), 2);
    EXPECT_EQ(handShake.getDocument(0).saslSupportedMechs[0],       "SCRAM-SHA-1");
    EXPECT_EQ(handShake.getDocument(0).saslSupportedMechs[1],       "SCRAM-SHA-256");
    EXPECT_EQ(handShake.getDocument(0).ok, 1.0);
}

