
#include <gtest/gtest.h>
#include "Op_Msg.h"
#include "test/OpTest.h"
#include <fstream>
#include <iterator>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

struct CRCTest
{
    int             whatsmyuri;
    std::string     $db;
};
ThorsAnvil_MakeTrait(CRCTest, whatsmyuri, $db);

TEST(Op_MsgTest, Op_MsgSerializeMessage)
{
    Op_MsgHeader::messageIdSetForTest(0xAF6789);
    SimpleStringNoConstructor                       object{"The String data"s};
    Op_Msg<Kind0<SimpleStringNoConstructor>>        message1{Kind0<SimpleStringNoConstructor>(object)};

    std::stringstream   stream;
    stream << message1 << std::flush;

    // https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg
    EXPECT_EQ(stream.str(),                     // Op_MsgHeader
                            "\x37\x00\x00\x00"      // size       Header(16) + Op_Msg(4 + Kind0 + 4) :  Note: Kind0 => 1 +  BSON(SimpleStringNoConstructor)
                            "\x89\x67\xAF\x00"      // requestId
                            "\x00\x00\x00\x00"      // always zero when we create it.
                            "\xDD\x07\x00\x00"      // OpCodea MSG => 2013
                                                // Op_Msg
                            "\x00\x00\x00\x00"      // flags
                                                    // Sections:
                            "\x00"                      // Kind0 1 byte marker
                                                        // BSON Object
                            "\x22\x00\x00\x00"                                                  // Size
                            "\x02" "message\x00"    "\x10\x00\x00\x00" "The String data\x00"    // Fields (1 in this case)
                            "\x00"s);                                                           // Zero Marker
}

TEST(Op_MsgTest, Op_MsgSerializeMessageValidateCheckSum)
{
    using IOSocketStream = ThorsAnvil::ThorsIO::IOSocketStream<MongoBuffer>;
    using DataSocket     = ThorsAnvil::ThorsIO::DataSocket;

    {
        Op_MsgHeader::messageIdSetForTest(1);
        Op_Msg<Kind0<CRCTest>>           message1(OP_MsgFlag::checksumPresent, CRCTest{1, "admin"});

        int             socket  = open("test/data/CRCTest", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
        DataSocket      dataSocket(socket);
        IOSocketStream  stream(dataSocket);

        stream << message1 << std::flush;
    }
    std::string result;
    {
        std::ifstream   test("test/data/CRCTest");
        std::string     line(std::istreambuf_iterator<char>{test}, std::istreambuf_iterator<char>{});
        result = line;
    }

    // https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg
    EXPECT_EQ(result,
                "\x3d\x00\x00\x00"              //      Size
                "\x01\x00\x00\x00"              //      Id
                "\x00\x00\x00\x00"              //      Reply
                "\xdd\x07\x00\x00"              //      OP_MSG
                                                //  OP_MSG
                "\x01\x00\x00\x00"              //  Flags   (add CheckSum)
                                                //  Sections
                "\x00"                          //      Kind 0
                "\x24\x00\x00\x00"              //      BSON Size
                "\x10"      "whatsmyuri\x00"      "\x01\x00\x00\x00"
                "\x02"      "$db\x00"             "\x06\x00\x00\x00" "admin\x00"
                "\x00"
                "\xbd\xac\xb0\x42"s
    );
    // "=\0\0\0\x89g\xAF\0\0\0\0\0\xDD\a\0\0\x1\0\0\0\0$\0\0\0\x10whatsmyuri\0\x1\0\0\0\x2$db\0\x6\0\0\0admin\0\0\0\0\0\0"
    // "=\0\0\0\x89g\xAF\0\0\0\0\0\xDD\a\0\0\x1\0\0\0\0$\0\0\0\x10whatsmyuri\0\x1\0\0\0\x2$db\0\x6\0\0\0admin\0\0\xBD\xAC\xB0" "B"
}

TEST(Op_MsgTest, Op_MsgSerializeMessageHumanReadable)
{
    Op_MsgHeader::messageIdSetForTest(0xAF6789);
    SimpleStringNoConstructor                       object{"The String data"s};
    Op_Msg<Kind0<SimpleStringNoConstructor>>        message1{Kind0<SimpleStringNoConstructor>(object)};

    std::stringstream   stream;
    stream << make_hr(message1);

    // https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg
    std::string line;
    std::size_t lineCount = 0;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }
    EXPECT_EQ(lineCount, 11);
}

TEST(Op_MsgTest, Op_MsgSerializeMessageValidateCheckSumHumanReadable)
{
    Op_MsgHeader::messageIdSetForTest(0xAF6789);
    Op_Msg<Kind0<SimpleStringNoConstructor>>           message1(OP_MsgFlag::checksumPresent, "The String data"s);

    std::stringstream   stream;
    stream << make_hr(message1);

    std::string line;
    std::size_t lineCount = 0;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }
    EXPECT_EQ(lineCount, 11);
}

