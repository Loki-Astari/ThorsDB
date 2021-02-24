
#include <gtest/gtest.h>
#include "Op_Msg.h"
#include "test/OpTest.h"
#include <fstream>
#include <iterator>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

struct CRCTest
{
    int             whatsmyuri  = 0;
    std::string     $db         = "Bob";
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
}

TEST(Op_MsgTest, Op_MsgSerializeMessageValidateCheckSumAndCompression)
{
    using IOSocketStream = ThorsAnvil::ThorsIO::IOSocketStream<MongoBuffer>;
    using DataSocket     = ThorsAnvil::ThorsIO::DataSocket;

    {
        Op_MsgHeader::messageIdSetForTest(1);
        Op_Msg<Kind0<CRCTest>>           message1(OP_MsgFlag::checksumPresent, CRCTest{1, "admin"});

        int                 socket  = open("test/data/CompCRCTest", O_WRONLY | O_CREAT | O_TRUNC, 0777 );
        DataSocket          dataSocket(socket);
        IOSocketStream      stream(dataSocket);

        // Add the compression buffer.
        MongoBuffer&        origBuffer = dynamic_cast<MongoBuffer&>(*stream.rdbuf());
        MongoBufferSnappy   buffer(std::move(origBuffer));
        stream.rdbuf(&buffer);

        // Make sure we don't sync the buffer until we have compressed the data.
        buffer.resizeOutputBuffer(message1.getMessageLength() + 9 + 5);

        // Make sure the object knows it is going to be compressed.
        message1.setCompression(1); // Snappy compression

        // Now Send the object to the stream.
        stream << message1 << std::flush;
    }
    std::string result;
    {
        std::ifstream   test("test/data/CompCRCTest");
        std::string     line(std::istreambuf_iterator<char>{test}, std::istreambuf_iterator<char>{});
        result = line;
    }

    // https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg
    EXPECT_EQ(result,
                                                // Header
                "\x47\x00\x00\x00"              //      Size
                "\x01\x00\x00\x00"              //      ID
                "\x00\x00\x00\x00"              //      Reply
                "\xdc\x07\x00\x00"              //      opCode: OP_COMPRESS
                                                // OP_COMPRESS MSG
                "\xdd\x07\x00\x00"              //      Original OpCode: OP_MSG
                "\x2d\x00\x00\x00"              //      Uncompressed Size
                "\x01"                          //      Compression Type (1 => Snappy)
                // Compressed Data
                "\x2d\x50\x01\x00\x00\x00\x00\x24\x00\x00\x00\x10\x77\x68\x61"
                "\x74\x73\x6d\x79\x75\x72\x69\x00\x01\x15\x4c\x02\x24\x64\x62"
                "\x00\x06\x00\x00\x00\x61\x64\x6d\x69\x6e\x00\x00"
                // CRC checksum
                "\xbd\xac\xb0\x42"s
    );
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

TEST(Op_MsgTest, Op_MsgSerializeMessageValidateCheckSumAndCompressionReadFromServer)
{
    using IOSocketStream = ThorsAnvil::ThorsIO::IOSocketStream<MongoBuffer>;
    using DataSocket     = ThorsAnvil::ThorsIO::DataSocket;

    Op_Msg<Kind0<CRCTest>>           message1;
    {

        int                 socket  = open("test/data/CompCRCTest-Read", O_RDONLY);
        DataSocket          dataSocket(socket);
        IOSocketStream      stream(dataSocket);

        // Add the compression buffer.
        MongoBuffer&        origBuffer = dynamic_cast<MongoBuffer&>(*stream.rdbuf());
        MongoBufferSnappy   buffer(std::move(origBuffer));
        stream.rdbuf(&buffer);

        // Now Send the object to the stream.
        stream >> message1;
    }
    EXPECT_EQ(message1.getDocument<0>().whatsmyuri, 1);
    EXPECT_EQ(message1.getDocument<0>().$db, "admin");
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

