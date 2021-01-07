

#include <gtest/gtest.h>
#include "Op_Query.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_QuertTest, Op_QueryStreamObject)
{
    MsgHeader::messageIdSetForTest(0x124589);
    StringAndIntNoConstructor               object{"DataString"s, 48};
   // Op_Query<StringAndIntNoConstructor>     query("thor.collection", object);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", object);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x00\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}

TEST(Op_QuertTest, Op_QueryStreamObjectHumanReadable)
{
    MsgHeader::messageIdSetForTest(0x124589);
    StringAndIntNoConstructor               object{"DataString"s, 48};
    Op_Query<StringAndIntNoConstructor>     query("thor.collection", object);

    std::stringstream stream;
    stream << make_hr(query);

    std::size_t lineCount = 0;
    std::string line;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }

    EXPECT_EQ(lineCount, 16);
}

