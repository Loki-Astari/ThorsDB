
#include <gtest/gtest.h>
#include "CmdDBTest.h"
// Ye-Old Wire Protocol
#include "Op_Insert.h"
#include "Op_Delete.h"
#include "Op_Query.h"
#include "Op_Reply.h"
#include "Op_GetMore.h"
#include "Op_KillCursors.h"
#include "Op_Update.h"
// Middle Wire Protocol
#include "CmdDB_Reply.h"
#include "CmdDB_Insert.h"
#include "CmdDB_Delete.h"
#include "CmdDB_Find.h"
#include "CmdDB_FindModify.h"
#include "CmdDB_GetLastError.h"
#include "CmdDB_GetMore.h"
// Other Stuff
#include "MongoConnection.h"
#include "MongoConfig.h"
#include "test/OpTest.h"

// Serialization
#include "ThorSerialize/JsonThor.h"

#include <iostream>
#include <vector>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;


/*
 * This test just verifies that Delete/Insert/Find work.
 *
 * Once we know this we can validate the commands with their own tests.
 * But we need to know these work so we can reset the DB to a clean state
 */
TEST_F(CmdDBTest, CmdDB_Delete_2_Items)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // Delete 2 item for the collection
    CmdDBTest::getConnection() << send_CmdDB_Delete("test", "ConnectionTest", FindValue{22});   // 22 matches 2 items

    std::size_t         count;
    CmdDB_DeleteReply   reply(count);
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk())
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(2, count);   // number deleted

    checkTheNumberofObjectsIs("CmdDB_Delete_2_Items", CmdDBTest::getConnection(), 3);
}
TEST_F(CmdDBTest, CmdDB_Delete_2_Items_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // Delete 2 item for the collection
    bool ok = false;
    std::size_t count;
    try
    {
        CmdDBTest::getConnection() << send_CmdDB_Delete("test", "ConnectionTest", FindValue{22});   // 22 matches 2 items
        CmdDBTest::getConnection() >> get_CmdDB_DeleteReply(count);
        ok = true;
    }
    catch(MongoException const& e)
    {
        std::cerr << e
                  << "\n\n";
    }

    EXPECT_TRUE(true);

    checkTheNumberofObjectsIs("CmdDB_Delete_2_Items", CmdDBTest::getConnection(), 3);
}

TEST_F(CmdDBTest, CmdDB_FindRemove_1_Items)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindRemove 1 item for the collection
    CmdDBTest::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{48});   // 2 items have 48 values this should remove one
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    bool item1 = result->message == "ThisAndThat";
    bool item2 = result->message == "DataString";
    if (!reply.isOk() || result.get() == nullptr || result->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(48,   result.get()->value);
    EXPECT_TRUE(item1 || item2);
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindRemove_1_Items", CmdDBTest::getConnection(), 4);
}

TEST_F(CmdDBTest, CmdDB_FindRemove_0_Items)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindRemove Remove an item that is not there.
    CmdDBTest::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{112});
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr, result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindRemove_0_Items", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_1_Items)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 1 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    bool item1 = result->message == "ThisAndThat";
    bool item2 = result->message == "DataString";
    if (!reply.isOk() || result.get() == nullptr || result->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(48,   result->value);
    EXPECT_TRUE(item1 || item2);
    EXPECT_TRUE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Items)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Item_And_Upsert)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert", CmdDBTest::getConnection(), 6);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_1_Items_ReturnUpdated)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 1 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk() || result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(0,   result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);
    EXPECT_TRUE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items_ReturnUpdated", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Items_ReturnNull)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items_ReturnNull", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true,.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk() || result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_NE(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);
    EXPECT_EQ(0, result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted", CmdDBTest::getConnection(), 6);
}

TEST_F(CmdDBTest, CmdDB_FindRemove_1_Items_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindRemove 1 item for the collection
    CmdDBTest::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{48});   // 2 items have 48 values this should remove one
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindDeleteReply(result);

    bool item1 = result->message == "ThisAndThat";
    bool item2 = result->message == "DataString";
    if (result.get() == nullptr || result->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(48,   result.get()->value);
    EXPECT_TRUE(item1 || item2);

    checkTheNumberofObjectsIs("CmdDB_FindRemove_1_Items", CmdDBTest::getConnection(), 4);
}

TEST_F(CmdDBTest, CmdDB_FindRemove_0_Items_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindRemove Remove an item that is not there.
    CmdDBTest::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{112});
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindDeleteReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr, result.get());

    checkTheNumberofObjectsIs("CmdDB_FindRemove_0_Items", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_1_Items_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 1 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    bool item1 = result->message == "ThisAndThat";
    bool item2 = result->message == "DataString";
    if (result.get() == nullptr || result->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(48,   result->value);
    EXPECT_TRUE(item1 || item2);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Items_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr,   result.get());

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Item_And_Upsert_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr,   result.get());

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert", CmdDBTest::getConnection(), 6);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_1_Items_ReturnUpdated_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 1 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(0,   result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items_ReturnUpdated", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Items_ReturnNull_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr,   result.get());

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items_ReturnNull", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    // FindUpdate 0 item for the collection

    CmdDBTest::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true,.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDBTest::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_NE(nullptr,   result.get());
    EXPECT_EQ(0, result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted", CmdDBTest::getConnection(), 6);
}

TEST_F(CmdDBTest, CmdDB_GetLastError_NoError)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    CmdDBTest::getConnection() << send_CmdDB_GetLastError("test");
    CmdDB_GetLastErrorReply   reply;
    CmdDBTest::getConnection() >> reply;

    if (!reply.isOk() || reply.reply.err.get() != nullptr || reply.reply.n != 0)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   reply.reply.err.get());
    EXPECT_EQ(0,         reply.reply.n);

    checkTheNumberofObjectsIs("CmdDB_GetLastError_NoError", CmdDBTest::getConnection(), 5);
}

TEST_F(CmdDBTest, CmdDB_GetMore)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    MongoConnection& connection = CmdDBTest::getConnection();

    std::vector<StringAndIntNoConstructor>      data;
    CmdDB_FindReply<StringAndIntNoConstructor>  reply1(data);
    connection << send_CmdDB_Find("test", "ConnectionTest", {.batchSize = 2});
    connection >> reply1;

    EXPECT_TRUE(reply1.isOk());
    EXPECT_EQ(2,   data.size());
    EXPECT_NE(0,   reply1.reply.cursor.id);
    EXPECT_TRUE(reply1.reply.cursor.partialResultsReturned);

    CmdDB_GetMoreReply<StringAndIntNoConstructor>   reply2(data);
    connection << send_CmdDB_GetMore("test", "ConnectionTest", {.batchSize = 2}, reply1);
    connection >> reply2;

    EXPECT_TRUE(reply2.isOk());
    EXPECT_EQ(4,   data.size());
    EXPECT_NE(0,   reply2.reply.cursor.id);
    EXPECT_TRUE(reply1.reply.cursor.partialResultsReturned);

    CmdDB_GetMoreReply<StringAndIntNoConstructor>   reply3(data);
    connection << send_CmdDB_GetMore("test", "ConnectionTest", {.batchSize = 2}, reply2);
    connection >> reply3;

    EXPECT_TRUE(reply3.isOk());
    EXPECT_EQ(5,   data.size());
    EXPECT_EQ(0,   reply3.reply.cursor.id);
    EXPECT_TRUE(reply1.reply.cursor.partialResultsReturned);

    CmdDB_GetMoreReply<StringAndIntNoConstructor>   reply4(data);
    connection << send_CmdDB_GetMore("test", "ConnectionTest", {.batchSize = 2}, reply2);
    connection >> reply4;

    EXPECT_FALSE(reply4.isOk());
}

TEST_F(CmdDBTest, CmdDB_GetMoreUsing_RValue)
{
#if defined(THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES) && (THOR_DISABLE_MONGO_TEST_SERVER_OP_CODES == 1)
    GTEST_SKIP() << "OP_QUERY No longer supported";
#endif
    MongoConnection& connection = CmdDBTest::getConnection();

    std::vector<StringAndIntNoConstructor>      data;
    connection << send_CmdDB_Find("test", "ConnectionTest", {.batchSize = 2});
    connection >> get_CmdDB_FindReply(data);

    EXPECT_EQ(2,   data.size());

    connection << send_CmdDB_GetMore("test", "ConnectionTest", {.batchSize = 2});
    connection >> get_CmdDB_GetMoreReply(data);

    EXPECT_EQ(4,   data.size());

    connection << send_CmdDB_GetMore("test", "ConnectionTest", {.batchSize = 2});
    connection >> get_CmdDB_GetMoreReply(data);

    EXPECT_EQ(5,   data.size());

    bool good = false;
    try
    {
        connection << send_CmdDB_GetMore("test", "ConnectionTest", {.batchSize = 2});
        connection >> get_CmdDB_GetMoreReply(data);
        good = true;
    }
    catch(MongoException const&)
    {}

    EXPECT_FALSE(good); // We expect the exception to be thrown
    EXPECT_EQ(5,   data.size());
}

