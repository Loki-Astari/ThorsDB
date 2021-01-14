

#include <gtest/gtest.h>
#include "Op_Insert.h"
#include "MongoConnection.h"
#include "MongoConfig.h"
#include "test/OpTest.h"

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_InsertTest, Op_InsertStreamObjectNoFlags)
{
    MsgHeader::messageIdSetForTest(0x12A5E9);
    StringAndIntNoConstructor               object1{"DataString"s, 48};
    StringAndIntNoConstructor               object2{"Another"s, 22};
    StringAndIntNoConstructor               object3{"ThirdAndLast"s, 0xFF};

    std::stringstream stream;
    stream << Op_Insert<StringAndIntNoConstructor>("thor.collection", object1, object2, object3);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x9B\x00\x00\x00"          // Size
                            "\xE9\xA5\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd2\x07\x00\x00"          // OpCode OP_INSERT 2002
                                                    // OP_Insert
                            "\x01\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s
                            "\x25\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x08\x00\x00\x00" "Another\x00"
                            "\x10"  "value\x00"     "\x16\x00\x00\x00"
                            "\x00"s
                            "\x2A\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0D\x00\x00\x00" "ThirdAndLast\x00"
                            "\x10"  "value\x00"     "\xFF\x00\x00\x00"
                            "\x00"s
                            );
}
TEST(Op_InsertTest, Op_InsertStreamObjectInsertErrorStop)
{
    MsgHeader::messageIdSetForTest(0x12A5E9);
    StringAndIntNoConstructor               object1{"DataString"s, 48};
    StringAndIntNoConstructor               object2{"Another"s, 22};
    StringAndIntNoConstructor               object3{"ThirdAndLast"s, 0xFF};

    std::stringstream stream;
    stream << Op_Insert<StringAndIntNoConstructor>("thor.collection", InsertError::Stop, object1, object2, object3);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x9B\x00\x00\x00"          // Size
                            "\xE9\xA5\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd2\x07\x00\x00"          // OpCode OP_INSERT 2002
                                                    // OP_Insert
                            "\x00\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s
                            "\x25\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x08\x00\x00\x00" "Another\x00"
                            "\x10"  "value\x00"     "\x16\x00\x00\x00"
                            "\x00"s
                            "\x2A\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0D\x00\x00\x00" "ThirdAndLast\x00"
                            "\x10"  "value\x00"     "\xFF\x00\x00\x00"
                            "\x00"s
                            );
}
TEST(Op_InsertTest, Op_InsertStreamObjectInsertErrorCont)
{
    MsgHeader::messageIdSetForTest(0x12A5E9);
    StringAndIntNoConstructor               object1{"DataString"s, 48};
    StringAndIntNoConstructor               object2{"Another"s, 22};
    StringAndIntNoConstructor               object3{"ThirdAndLast"s, 0xFF};

    std::stringstream stream;
    stream << Op_Insert<StringAndIntNoConstructor>("thor.collection", InsertError::Cont, object1, object2, object3);

    EXPECT_EQ(stream.str(),                         // Message Header
                            "\x9B\x00\x00\x00"          // Size
                            "\xE9\xA5\x12\x00"          // Id
                            "\x00\x00\x00\x00"          // Response ID
                            "\xd2\x07\x00\x00"          // OpCode OP_INSERT 2002
                                                    // OP_Insert
                            "\x01\x00\x00\x00"          // Flags
                            "thor.collection\x00"      // Collection -> See constructo
                                                    // BSON document (the query) of StringAndIntNoConstructor
                            "\x28\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0B\x00\x00\x00" "DataString\x00"
                            "\x10"  "value\x00"     "\x30\x00\x00\x00"
                            "\x00"s
                            "\x25\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x08\x00\x00\x00" "Another\x00"
                            "\x10"  "value\x00"     "\x16\x00\x00\x00"
                            "\x00"s
                            "\x2A\x00\x00\x00"          // Size
                            "\x02"  "message\x00"   "\x0D\x00\x00\x00" "ThirdAndLast\x00"
                            "\x10"  "value\x00"     "\xFF\x00\x00\x00"
                            "\x00"s
                            );
}

TEST(Op_InsertTest, Op_InsertStreamObjectHumanReadable)
{
    MsgHeader::messageIdSetForTest(0x124589);
    StringAndIntNoConstructor               object1{"DataString"s, 48};
    StringAndIntNoConstructor               object2{"Another"s, 22};
    StringAndIntNoConstructor               object3{"ThirdAndLast"s, 0xFF};
    Op_Insert<StringAndIntNoConstructor>    query("thor.collection", object1, object2, object3);

    std::stringstream stream;
    stream << make_hr(query);

    std::size_t lineCount = 0;
    std::string line;
    while(std::getline(stream, line))
    {
        ++lineCount;
    }

    EXPECT_EQ(lineCount, 21);
}

