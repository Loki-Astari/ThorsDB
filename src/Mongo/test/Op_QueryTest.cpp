

#include <gtest/gtest.h>
#include "Op_Query.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_QueryTest, Op_QueryStreamObjectNoFlag)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorLeaveOpen)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", QueryOptions{}.leaveOpen(), "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x02\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorSlaveOk)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", QueryOptions{}.slaveOK(), "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x04\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorOpLogNoReplay)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", QueryOptions{}.opLogNoReplay(), "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x08\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorCurserNoTimeout)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", QueryOptions{}.curserNoTimeout(), "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x10\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorDataTimeout)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", QueryOptions{}.dataTimeout(), "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x20\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorDrainAll)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", QueryOptions{}.drainAll(), "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x40\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorPartialAvailable)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", QueryOptions{}.partialAvailable(), "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\x80\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorAll)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection",
                                                  QueryOptions{}.leaveOpen().slaveOK().opLogNoReplay().curserNoTimeout()
                                                                .dataTimeout().drainAll().partialAvailable(),
                                                  "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x54\x00\x00\x00"          // Size
                            "\x89\x45\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd4\x07\x00\x00"          // OpCode OP_QUERY 2004
                                                    // OP_Query
                            "\xFE\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                            "\x00\x00\x00\x00"          // numberToSkip
                            "\x01\x00\x00\x00"          // numberToReturn
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s);
}

TEST(Op_QueryTest, Op_QueryStreamObjectHumanReadable)
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

