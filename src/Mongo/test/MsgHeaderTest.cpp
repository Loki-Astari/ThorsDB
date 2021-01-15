
#include <gtest/gtest.h>
#include "MsgHeader.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(MsgHeaderTest, CreateReply)
{
    // Should simply create.
    MsgHeader   message(OpCode::OP_REPLY);
}

TEST(MsgHeaderTest, CheckEquality)
{
    MsgHeader   messageOne(OpCode::OP_UPDATE);
    MsgHeader   messageTwo(OpCode::OP_UPDATE);

    EXPECT_EQ(messageOne, messageTwo);
}

TEST(MsgHeaderTest, CheckNotEquality)
{
    MsgHeader   messageOne(OpCode::OP_INSERT);
    MsgHeader   messageTwo(OpCode::OP_QUERY);

    EXPECT_NE(messageOne, messageTwo);
}

TEST(MsgHeaderTest, SetDataSizeSerialize)
{
    MsgHeader   messageOne(OpCode::OP_GET_MORE);
    MsgHeader::messageIdSetForTest(0x23456789);
    messageOne.prepareToSend(15);   // Not size is 15 for the data.
                                    // The header contains the size of the data + the header.
                                    // 15  + sizeof(MsgHeader) i.e. 15+16 => 31 (0x1F)

    std::stringstream message;
    message << messageOne;

    std::string output = message.str();

    EXPECT_EQ("\x1F\x00\x00\x00"    // size
              "\x89\x67\x45\x23"    // requestId
              "\x00\x00\x00\x00"    // always zero when we create it.
              "\xD5\x07\x00\x00"s,  // OpCodea GET_MORE => 2005
              output);
}

TEST(MsgHeaderTest, SerializeDeSerialize)
{
    MsgHeader   messageOne(OpCode::OP_DELETE);
    MsgHeader::messageIdSetForTest(0xAFBECD);
    messageOne.prepareToSend(0x234589); // Not size is 15 for the data.
                                        // The header contains the size of the data + the header.
                                        // 0x234589  + sizeof(MsgHeader) i.e. 0x234589+16 => 2311577 (0x0x234599)

    std::stringstream message;
    message << messageOne;

    std::string output = message.str();

    EXPECT_EQ("\x99\x45\x23\x00"    // size
              "\xCD\xBE\xAF\x00"    // requestId
              "\x00\x00\x00\x00"    // always zero when we create it.
              "\xD6\x07\x00\x00"s,  // OpCodea OP_DELETE => 2006
              output);

    MsgHeader   messageTwo;
    message >> messageTwo;

    EXPECT_EQ(messageOne, messageTwo);
}

TEST(MsgHeaderTest, SerializeHumanReadable)
{
    MsgHeader   messageOne(OpCode::OP_KILL_CURSORS);
    MsgHeader::messageIdSetForTest(0xAF0000A1);
    messageOne.prepareToSend(0xFE1200); // Not size is 15 for the data.
                                        // The header contains the size of the data + the header.
                                        // 0xFE1200  + sizeof(MsgHeader) i.e. 0xFE1200+16 => 16650768 (0x0xFE1210)

    std::stringstream message;
    message << make_hr(messageOne);

    int lineCount = 0;
    std::string line;
    while(std::getline(message, line))
    {
        ++lineCount;
    }
    EXPECT_EQ(4, lineCount);
}

