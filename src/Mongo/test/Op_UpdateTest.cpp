

#include <gtest/gtest.h>
#include "Op_Update.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_UpdateTest, Op_UpdateStreamObjectNoFlag)
{
    MsgHeader::messageIdSetForTest(0x184A89);

    std::stringstream stream;
    stream << Op_Update<SimpleStringNoConstructor, StringAndIntNoConstructor>("thor.collection", SimpleStringNoConstructor{"Another"}, StringAndIntNoConstructor{"Another", 56});

    EXPECT_EQ(stream.str(),                                     // Message Header
                            "\x67\x00\x00\x00"                      // Size 32
                            "\x89\x4A\x18\x00"                      // Id
                            "\x00\x00\x00\x00"                      // Response ID
                            "\xd1\x07\x00\x00"                      // OpCode OP_UPDATE 2001
                                                                // OP_Update
                            "\x00\x00\x00\x00"                      // ZERO
                            "thor.collection\x00"                   // Collection -> See constructo
                            "\x00\x00\x00\x00"                      // Flags
                            // BSON Selector    => SimpleStringNoConstructor
                            "\x1A\x00\x00\x00"
                            "\x02"  "message\x00"   "\x08\x00\x00\x00"  "Another\x00"
                            "\x00"
                            // BSON Updater     => StringAndIntNoConstructor
                            "\x25\x00\x00\x00"
                            "\x02"  "message\x00"   "\x08\x00\x00\x00"  "Another\x00"
                            "\x10"  "value\x00"     "\x38\x00\x00\x00"
                            "\x00"
                            ""s);
}

TEST(Op_UpdateTest, Op_UpdateStreamObjectHumanReadable)
{
    MsgHeader::messageIdSetForTest(0x124589);
    Op_Update<SimpleStringNoConstructor, StringAndIntNoConstructor>   update("thor.collection", SimpleStringNoConstructor{"Another"}, StringAndIntNoConstructor{"Another", 56});

    std::stringstream stream;
    stream << make_hr(update);

    std::size_t lineCount = 0;
    std::string line;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }

    EXPECT_EQ(lineCount, 15);
}

