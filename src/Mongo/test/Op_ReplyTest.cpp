

#include <gtest/gtest.h>
#include "Op_Reply.h"
#include "test/OpTest.h"

struct OpReplyData
{
    std::int32_t    data1;
    std::int32_t    data2;
    std::int32_t    data3;
};

ThorsAnvil_MakeTrait(OpReplyData, data1, data2, data3);

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

TEST(Op_ReplyTest, Op_ReplyStreamGoodResponse)
{
    std::string data =      "\x70\x00\x00\x00"                      // Size
                            "\x89\x4A\x18\x00"                      // Id
                            "\x23\x21\xAA\x4B"                      // Response ID
                            "\xd4\x07\x00\x00"                      // OpCode OP_QUERY 2004
                                                                // OP_REPLY
                            "\x00\x00\x00\x00"                      // responseFlags
                            "\x12\x34\x56\x78\x9A\xBC\xDE\xF0"      // cursorID
                            "\x00\x00\x00\x00"                      // startingFrom
                            "\x02\x00\x00\x00"                      // numberReturned
                            // BSON
                            "\x26\x00\x00\x00"
                            "\x10"  "data1\x00" "\x01\x00\x00\x00"
                            "\x10"  "data2\x00" "\x02\x00\x00\x00"
                            "\x10"  "data3\x00" "\x03\x00\x00\x00"
                            "\x00"
                            "\x26\x00\x00\x00"
                            "\x10"  "data1\x00" "\x04\x00\x00\x00"
                            "\x10"  "data2\x00" "\x05\x00\x00\x00"
                            "\x10"  "data3\x00" "\x06\x00\x00\x00"
                            "\x00"
                            ""s;


    std::stringstream stream(data);
    std::vector<OpReplyData>    result;

    Op_Reply<std::vector<OpReplyData>>  reply(result);
    stream >> reply;

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(2, result.size());
    EXPECT_EQ(1, result[0].data1);
    EXPECT_EQ(2, result[0].data2);
    EXPECT_EQ(3, result[0].data3);
    EXPECT_EQ(4, result[1].data1);
    EXPECT_EQ(5, result[1].data2);
    EXPECT_EQ(6, result[1].data3);
}

TEST(Op_ReplyTest, Op_ReplyQueryFailureWithError)
{
    std::string data =      "\x6D\x00\x00\x00"                      // Size
                            "\x89\x4A\x18\x00"                      // Id
                            "\x23\x21\xAA\x4B"                      // Response ID
                            "\xd4\x07\x00\x00"                      // OpCode OP_QUERY 2004
                                                                // OP_REPLY
                            "\x02\x00\x00\x00"                      // responseFlags
                            "\x12\x34\x56\x78\x9A\xBC\xDE\xF0"      // cursorID
                            "\x00\x00\x00\x00"                      // startingFrom
                            "\x01\x00\x00\x00"                      // numberReturned
                            // BSON
                            "\x49\x00\x00\x00"
                            "\x01"  "ok\x00"        "\x00\x00\x00\x00\x00\x00\x00\x00"      // 12
                            "\x10"  "code\x00"      "\x22\x00\x00\x00"                      // 10
                            "\x02"  "$err\x00"      "\x0C\x00\x00\x00"  "Error Sting\x00"   // 22
                            "\x02"  "codeName\x00"  "\x0A\x00\x00\x00"  "Code Name\x00"     // 24
                            "\x00"
                            ""s;


    std::stringstream stream(data);
    std::vector<OpReplyData>    result;

    Op_Reply<std::vector<OpReplyData>>  reply(result);
    stream >> reply;

    EXPECT_FALSE(reply.isOk());
    EXPECT_EQ(0, result.size());
    EXPECT_EQ("Op_Reply: 34: Code Name: Error Sting", reply.getHRErrorMessage());
}

TEST(Op_ReplyTest, Op_ReplyCursorNotFoundWithError)
{
    std::string data =      "\x24\x00\x00\x00"                      // Size
                            "\x89\x4A\x18\x00"                      // Id
                            "\x23\x21\xAA\x4B"                      // Response ID
                            "\xd4\x07\x00\x00"                      // OpCode OP_QUERY 2004
                                                                // OP_REPLY
                            "\x01\x00\x00\x00"                      // responseFlags
                            "\x12\x34\x56\x78\x9A\xBC\xDE\xF0"      // cursorID
                            "\x00\x00\x00\x00"                      // startingFrom
                            "\x00\x00\x00\x00"                      // numberReturned
                            ""s;


    std::stringstream stream(data);
    std::vector<OpReplyData>    result;

    Op_Reply<std::vector<OpReplyData>>  reply(result);
    stream >> reply;

    EXPECT_FALSE(reply.isOk());
    EXPECT_EQ(0, result.size());
    EXPECT_EQ("Op_Reply: -1: : CursorNotFound", reply.getHRErrorMessage());
}
/// ----
TEST(Op_ReplyTest, Op_ReplyStreamGoodResponseDynamic)
{
    std::string data =      "\x70\x00\x00\x00"                      // Size
                            "\x89\x4A\x18\x00"                      // Id
                            "\x23\x21\xAA\x4B"                      // Response ID
                            "\xd4\x07\x00\x00"                      // OpCode OP_QUERY 2004
                                                                // OP_REPLY
                            "\x00\x00\x00\x00"                      // responseFlags
                            "\x12\x34\x56\x78\x9A\xBC\xDE\xF0"      // cursorID
                            "\x00\x00\x00\x00"                      // startingFrom
                            "\x02\x00\x00\x00"                      // numberReturned
                            // BSON
                            "\x26\x00\x00\x00"
                            "\x10"  "data1\x00" "\x01\x00\x00\x00"
                            "\x10"  "data2\x00" "\x02\x00\x00\x00"
                            "\x10"  "data3\x00" "\x03\x00\x00\x00"
                            "\x00"
                            "\x26\x00\x00\x00"
                            "\x10"  "data1\x00" "\x04\x00\x00\x00"
                            "\x10"  "data2\x00" "\x05\x00\x00\x00"
                            "\x10"  "data3\x00" "\x06\x00\x00\x00"
                            "\x00"
                            ""s;

    std::stringstream stream(data);
    bool error = true;

    try
    {
        std::vector<OpReplyData>    result;

        stream >> make_Op_Reply(result);

        EXPECT_EQ(2, result.size());
        EXPECT_EQ(1, result[0].data1);
        EXPECT_EQ(2, result[0].data2);
        EXPECT_EQ(3, result[0].data3);
        EXPECT_EQ(4, result[1].data1);
        EXPECT_EQ(5, result[1].data2);
        EXPECT_EQ(6, result[1].data3);
        error = false;
    }
    catch(...){}
    EXPECT_FALSE(error);
}

TEST(Op_ReplyTest, Op_ReplyQueryFailureWithErrorDynamic)
{
    std::string data =      "\x6D\x00\x00\x00"                      // Size
                            "\x89\x4A\x18\x00"                      // Id
                            "\x23\x21\xAA\x4B"                      // Response ID
                            "\xd4\x07\x00\x00"                      // OpCode OP_QUERY 2004
                                                                // OP_REPLY
                            "\x02\x00\x00\x00"                      // responseFlags
                            "\x12\x34\x56\x78\x9A\xBC\xDE\xF0"      // cursorID
                            "\x00\x00\x00\x00"                      // startingFrom
                            "\x01\x00\x00\x00"                      // numberReturned
                            // BSON
                            "\x49\x00\x00\x00"
                            "\x01"  "ok\x00"        "\x00\x00\x00\x00\x00\x00\x00\x00"      // 12
                            "\x10"  "code\x00"      "\x22\x00\x00\x00"                      // 10
                            "\x02"  "$err\x00"      "\x0C\x00\x00\x00"  "Error Sting\x00"   // 22
                            "\x02"  "codeName\x00"  "\x0A\x00\x00\x00"  "Code Name\x00"     // 24
                            "\x00"
                            ""s;

    std::stringstream stream(data);
    std::vector<OpReplyData>    result;

    try
    {
        stream >> make_Op_Reply(result);
        EXPECT_TRUE(false);
    }
    catch(MongoException const& e)
    {
        EXPECT_EQ(0, result.size());
        EXPECT_EQ("34: Code Name: Error Sting", e.info.getHRErrorMessage());
    }
}

TEST(Op_ReplyTest, Op_ReplyCursorNotFoundWithErrorDynamic)
{
    std::string data =      "\x24\x00\x00\x00"                      // Size
                            "\x89\x4A\x18\x00"                      // Id
                            "\x23\x21\xAA\x4B"                      // Response ID
                            "\xd4\x07\x00\x00"                      // OpCode OP_QUERY 2004
                                                                // OP_REPLY
                            "\x01\x00\x00\x00"                      // responseFlags
                            "\x12\x34\x56\x78\x9A\xBC\xDE\xF0"      // cursorID
                            "\x00\x00\x00\x00"                      // startingFrom
                            "\x00\x00\x00\x00"                      // numberReturned
                            ""s;

    std::stringstream stream(data);
    std::vector<OpReplyData>    result;

    try
    {
        stream >> make_Op_Reply(result);
        EXPECT_TRUE(false);
    }
    catch(MongoException const& e)
    {
        EXPECT_EQ(0, result.size());
        EXPECT_EQ("-1: : CursorNotFound", e.info.getHRErrorMessage());
    }
}

