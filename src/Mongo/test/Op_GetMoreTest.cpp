

#include <gtest/gtest.h>
#include "Op_GetMore.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_GetMoreTest, Op_GetMoreStreamObjectNoFlag)
{
    MsgHeader::messageIdSetForTest(0x184A89);

    std::stringstream stream;
    stream << Op_GetMore<StringAndIntNoConstructor>("thor.collection", 100, 0x123456789ABCDEF0);

    EXPECT_EQ(stream.str(),                                     // Message Header
                            "\x30\x00\x00\x00"                      // Size 32
                            "\x89\x4A\x18\x00"                      // Id
                            "\x00\x00\x00\x00"                      // Response ID
                            "\xd5\x07\x00\x00"                      // OpCode OP_GET_MORE 2005
                                                                // OP_GetMore
                            "\x00\x00\x00\x00"                      // ZERO
                            "thor.collection\x00"                   // Collection -> See constructo
                            "\x64\x00\x00\x00"                      // numberToReturn
                            "\xF0\xDE\xBC\x9A\x78\x56\x34\x12"      // CurserId
                            ""s);
}

TEST(Op_GetMoreTest, Op_GetMoreStreamObjectHumanReadable)
{
    MsgHeader::messageIdSetForTest(0x124589);
    Op_GetMore<StringAndIntNoConstructor>   getMore("thor.collection", 23, 0);

    std::stringstream stream;
    stream << make_hr(getMore);

    std::size_t lineCount = 0;
    std::string line;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }

    EXPECT_EQ(lineCount, 8);
}

