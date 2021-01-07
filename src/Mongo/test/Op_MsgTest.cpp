
#include <gtest/gtest.h>
#include "Op_Msg.h"
#include "test/Op_MsgTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_MsgTest, Op_MsgCreateSimpleNoConstructor)
{
    OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>           message1("Data"s);
}
TEST(Op_MsgTest, Op_MsgCreateSimpleConstructor)
{
    OP_Msg<Kind0<Kind0SimpleStringWithConstructor>>         message1("Data"s);
}
TEST(Op_MsgTest, Op_MsgCreateStringAndIntNoConstructor)
{
    OP_Msg<Kind0<Kind0StringAndIntNoConstructor>>           message1(Kind0StringAndIntNoConstructor{"Data"s, 12});
}
TEST(Op_MsgTest, Op_MsgCreateStringAndIntConstructor)
{
    OP_Msg<Kind0<Kind0StringAndIntWithConstructor>>         message1(Kind0StringAndIntWithConstructor{"Data"s, 13});
}

TEST(Op_MsgTest, Op_MsgSerializeMessage)
{
    MsgHeader::messageIdSetForTest(0xAF6789);
    OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>           message1("The String data"s);

    std::stringstream   stream;
    stream << message1;

    // https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg
    EXPECT_EQ(stream.str(),                     // MsgHeader
                            "\x37\x00\x00\x00"      // size       Header(16) + Kind0(4 + Kind0 + 4) :  Note: Kind0 => 1 +  BSON(Kind0SimpleStringNoConstructor)
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
    MsgHeader::messageIdSetForTest(0xAF6789);
    OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>           message1("The String data"s);

    std::stringstream   stream;
    stream << message1;

    // https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/#op-msg
    EXPECT_EQ(stream.str(),                     // MsgHeader
                            "\x37\x00\x00\x00"      // size       Header(16) + Kind0(4 + Kind0 + 4) :  Note: Kind0 => 1 +  BSON(Kind0SimpleStringNoConstructor)
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
    MsgHeader::messageIdSetForTest(0xAF6789);
    OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>           message1("The String data"s);

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
    MsgHeader::messageIdSetForTest(0xAF6789);
    OP_Msg<Kind0<Kind0SimpleStringNoConstructor>>           message1("The String data"s);

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

