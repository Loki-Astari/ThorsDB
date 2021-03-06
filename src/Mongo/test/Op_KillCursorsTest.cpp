

#include <gtest/gtest.h>
#include "Op_KillCursors.h"
#include "Op_Reply.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_KillCursorsTest, Op_KillCursorsStreamObjectNoFlag)
{
    Op_MsgHeader::messageIdSetForTest(0x184A89);
    int             data1;
    auto            reply1 = get_Op_Reply(data1);
    reply1.cursorIDSetForTest(0x123456789ABCDEF0L);

    std::stringstream stream;
    stream << send_Op_KillCursors(reply1);

    EXPECT_EQ(stream.str(),                                     // Message Header
                            "\x20\x00\x00\x00"                      // Size
                            "\x89\x4A\x18\x00"                      // Id
                            "\x00\x00\x00\x00"                      // Response ID
                            "\xd7\x07\x00\x00"                      // OpCode OP_KILL_CURSORS 2007
                                                                // OP_KillCursors
                            "\x00\x00\x00\x00"                      // ZERO
                            "\x01\x00\x00\x00"                      // numberToCursors
                            "\xF0\xDE\xBC\x9A\x78\x56\x34\x12"      // CurserId
                            ""s);
}

TEST(Op_KillCursorsTest, Op_KillCursorsStreamObjectHumanReadable)
{
    Op_MsgHeader::messageIdSetForTest(0x124589);
    int             data1;
    auto            reply1 = get_Op_Reply(data1);
    reply1.cursorIDSetForTest(0x123456789ABCDEF0L);


    std::stringstream stream;
    stream << make_hr(send_Op_KillCursors(reply1));

    std::size_t lineCount = 0;
    std::string line;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }

    EXPECT_EQ(lineCount, 7);
}

