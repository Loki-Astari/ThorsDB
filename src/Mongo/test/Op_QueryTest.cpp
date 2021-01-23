

#include <gtest/gtest.h>
#include "Op_Query.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_QueryTest, Op_QueryStreamObjectNoFlag2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorLeaveOpen2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {.tailableCursor = TailableCursor::LeaveOpen}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorSlaveOk2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {.slave = Slave::OK}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorOpLogNoReplay2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {.oplog = Oplog::NoReplay}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorCurserNoTimeout2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {.curser = Curser::NoTimeout}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorDataTimeout2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {.data = Data::Timeout}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorDrainAll2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {.drain = Drain::All}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorPartialAvailable2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection", {.partial = Partial::Available}, 1, 0, "DataString"s, 48);

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
TEST(Op_QueryTest, Op_QueryStreamObjectFlag_CursorAll2)
{
    MsgHeader::messageIdSetForTest(0x124589);

    std::stringstream stream;
    stream << Op_Query<StringAndIntNoConstructor>("thor.collection",
                                                  {.tailableCursor  = TailableCursor::LeaveOpen,
                                                   .slave           = Slave::OK,
                                                   .oplog           = Oplog::NoReplay,
                                                   .curser          = Curser::NoTimeout,
                                                   .data            = Data::Timeout,
                                                   .drain           = Drain::All,
                                                   .partial         = Partial::Available,
                                                  },
                                                  1, 0, "DataString"s, 48);

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

TEST(Op_QueryTest, Op_QueryStreamObjectHumanReadable2)
{
    MsgHeader::messageIdSetForTest(0x124589);
    StringAndIntNoConstructor               object{"DataString"s, 48};
    Op_Query<StringAndIntNoConstructor>     query("thor.collection", {}, 1, 0, object);

    std::stringstream stream;
    stream << make_hr(query);

    std::size_t lineCount = 0;
    std::string line;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }

    EXPECT_EQ(lineCount, 13);
}

