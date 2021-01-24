

#include <gtest/gtest.h>
#include "Op_Delete.h"
#include "MongoConnection.h"
#include "MongoConfig.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_DeleteTest, Op_DeleteStreamObjectNoFlag)
{
    MsgHeader::messageIdSetForTest(0xA2A5F5);

    std::stringstream stream;
    stream << Op_Delete<StringAndIntNoConstructor>("thor.collection", {}, "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x50\x00\x00\x00"          // Size
                            "\xF5\xA5\xA2\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd6\x07\x00\x00"          // OpCode OP_DELETE 2006
                                                    // OP_Delete
                            "\x00\x00\x00\x00"          // zero
                            "thor.collection\x00"       // Collection -> See constructor
                            "\x00\x00\x00\x00"          // Flags
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s
                            );
}
TEST(Op_DeleteTest, Op_DeleteStreamObjectRemoveSingle)
{
    MsgHeader::messageIdSetForTest(0xA2A5F5);

    std::stringstream stream;
    stream << Op_Delete<StringAndIntNoConstructor>("thor.collection", {.flags = OP_DeleteFlag::SingleRemove}, "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x50\x00\x00\x00"          // Size
                            "\xF5\xA5\xA2\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd6\x07\x00\x00"          // OpCode OP_DELETE 2006
                                                    // OP_Delete
                            "\x00\x00\x00\x00"          // zero
                            "thor.collection\x00"       // Collection -> See constructor
                            "\x01\x00\x00\x00"          // Flags
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s
                            );
}
TEST(Op_DeleteTest, Op_DeleteStreamObjectRemoveMultiple)
{
    MsgHeader::messageIdSetForTest(0xA2A5F5);

    std::stringstream stream;
    stream << Op_Delete<StringAndIntNoConstructor>("thor.collection", {}, "DataString"s, 48);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x50\x00\x00\x00"          // Size
                            "\xF5\xA5\xA2\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd6\x07\x00\x00"          // OpCode OP_DELETE 2006
                                                    // OP_Delete
                            "\x00\x00\x00\x00"          // zero
                            "thor.collection\x00"       // Collection -> See constructor
                            "\x00\x00\x00\x00"          // Flags
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s
                            );
}

TEST(Op_DeleteTest, Op_DeleteStreamObjectHumanReadable)
{
    MsgHeader::messageIdSetForTest(0x124589);
    Op_Delete<StringAndIntNoConstructor>    query("thor.collection", {}, "DataString"s, 48);

    std::stringstream stream;
    stream << make_hr(query);

    std::size_t lineCount = 0;
    std::string line;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }

    EXPECT_EQ(lineCount, 12);
}

