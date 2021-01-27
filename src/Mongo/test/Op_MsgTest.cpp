
#include <gtest/gtest.h>
#include "Op_Msg.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_MsgTest, Op_MsgSerializeMessage)
{
    Op_MsgHeader::messageIdSetForTest(0xAF6789);
    SimpleStringNoConstructor                       object{"The String data"s};
    Op_Msg<Kind0<SimpleStringNoConstructor>>        message1{{}, Kind0<SimpleStringNoConstructor>(object)};

    std::stringstream   stream;
    stream << message1;

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

#if 0
TEST(Op_MsgTest, Op_MsgSerializeMessageValidateCheckSum)
{
    Op_MsgHeader::messageIdSetForTest(0xAF6789);
    Op_Msg<Kind0<SimpleStringNoConstructor>>           message1("The String data"s);

    std::stringstream   stream;
    stream << message1;

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
                            "\x00"                                                             // Zero Marker

                            "\x00\x00\x00\x00"s);   // Checksum
}
#endif

TEST(Op_MsgTest, Op_MsgSerializeMessageHumanReadable)
{
    Op_MsgHeader::messageIdSetForTest(0xAF6789);
    SimpleStringNoConstructor                       object{"The String data"s};
    Op_Msg<Kind0<SimpleStringNoConstructor>>        message1{{}, Kind0<SimpleStringNoConstructor>(object)};

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

#if 0
TEST(Op_MsgTest, Op_MsgSerializeMessageValidateCheckSum)
{
    Op_MsgHeader::messageIdSetForTest(0xAF6789);
    Op_Msg<Kind0<SimpleStringNoConstructor>>           message1("The String data"s);

    std::stringstream   make_hr(stream);
    stream << message1;

    std::String line;
    std::size_t lineCount = 0;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }
    EXPECT_EQ(lineCount, 11);
}
#endif

