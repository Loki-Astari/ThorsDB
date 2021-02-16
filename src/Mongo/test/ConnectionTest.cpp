
#include <gtest/gtest.h>
#include "UnitTestWithConnection.h"
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

TEST(ConnectionTest, YeOldWireProtocol)
{
    using std::string_literals::operator""s;
    using namespace ThorsAnvil::DB::Mongo;

    MongoConnection  connection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});

    std::string fullConnection  = THOR_TESTING_MONGO_DB;
    fullConnection += ".ConnectionTest";

    // reset the collection to be empty.
    {
        connection << send_Op_Delete(fullConnection, TestFindAll{});
    }

    // Make sure there are zero objects in the collection.
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.skip = 100, .ret = 0});
        std::vector<StringAndIntNoConstructorReply>   replyData;

        try
        {
            connection >> get_Op_Reply(replyData);
            EXPECT_EQ(0, replyData.size());
        }
        catch(MongoException const& e)
        {
            std::cerr << e
                      << ThorsAnvil::Serialize::jsonExporter(replyData)
                      << "\n\n";
        }
    }

    // Add three objects to the collection
    {
        std::vector<StringAndIntNoConstructor>  data({{"DataString"s, 48},
                                                      {"Another"s, 22},
                                                      {"ThirdAndLast"s, 0xFF}});

        connection << send_Op_Insert(fullConnection, data);
    }

    // Check there are three objects in the collection.
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.ret = 100});
        std::vector<StringAndIntNoConstructorReply> replyData;

        try
        {
            connection >> get_Op_Reply(replyData);
            EXPECT_EQ(3, replyData.size());
        }
        catch(MongoException const& e)
        {
            std::cerr << e
                      << ThorsAnvil::Serialize::jsonExporter(replyData)
                      << "\n\n";
        }
    }

    // Get the three objects one at time using GET_MORE
    {
        std::vector<StringAndIntNoConstructorReply>             replyData1;
        std::vector<StringAndIntNoConstructorReply>             replyData2;
        std::vector<StringAndIntNoConstructorReply>             replyData3;

        try
        {
            connection << Op_Query<TestFindAll>(fullConnection, {.ret = 2});

            connection >> get_Op_Reply(replyData1);
            EXPECT_EQ(2, replyData1.size());
            auto cursorInfo1 = connection.getCursorInfo(connection.getLastOpenCursor());
            EXPECT_EQ(cursorInfo1.first, 0);
            EXPECT_EQ(cursorInfo1.second, 2);

            connection << send_Op_GetMore(fullConnection, 1);

            connection >> get_Op_Reply(replyData2);
            EXPECT_EQ(1, replyData2.size());
            auto cursorInfo2 = connection.getCursorInfo(connection.getLastOpenCursor());
            EXPECT_EQ(cursorInfo2.first, 0);
            EXPECT_EQ(cursorInfo2.second, 3);

            connection << send_Op_GetMore(fullConnection, 1);

            connection >> get_Op_Reply(replyData3);
            EXPECT_EQ(0, replyData3.size());
            auto cursorInfo3 = connection.getCursorInfo(connection.getLastOpenCursor());
            EXPECT_EQ(cursorInfo3.first, 0);
            EXPECT_EQ(cursorInfo3.second, 3);
        }
        catch(MongoException const& e)
        {
            std::cerr << e
                      << ThorsAnvil::Serialize::jsonExporter(replyData1)
                      << ThorsAnvil::Serialize::jsonExporter(replyData2)
                      << ThorsAnvil::Serialize::jsonExporter(replyData3)
                      << "\n\n";
        }
    }
    // Get the two of three objects then kill cursor
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.ret = 2});

        std::vector<StringAndIntNoConstructorReply>     replyData1;
        auto                                            reply1 = get_Op_Reply(replyData1);
        connection >> reply1;

        EXPECT_TRUE(reply1.isOk());
        EXPECT_EQ(reply1.startingFrom, 0);
        EXPECT_EQ(reply1.numberReturned, 2);
        ASSERT_EQ(reply1.documents.size(), 2);

        connection << send_Op_KillCursors(reply1);

        connection << send_Op_GetMore(fullConnection, reply1);

        StringAndIntNoConstructorReply                  replyData2;
        auto                                            reply2  = get_Op_Reply(replyData2);
        connection >> reply2;
        EXPECT_FALSE(reply2.isOk());
        EXPECT_EQ(reply2.responseFlags & OP_ReplyFlag::CursorNotFound, OP_ReplyFlag::CursorNotFound);
    }

    // Check the query can actual filter.
    {
        connection << Op_Query<SimpleStringNoConstructor>(fullConnection, {.ret = 100}, "Another");
        StringAndIntNoConstructorReply                  replyData;
        auto                                            reply = get_Op_Reply(replyData);
        connection >> reply;

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1, reply.numberReturned);
        if (!reply.isOk() || reply.numberReturned != 1)
        {
            std::cerr << "Showing: " << reply.numberReturned << "\n";
            for (int loop = 0; loop < reply.numberReturned; ++loop)
            {
                std::cerr << "Item: " << loop << "\n"
                          << ThorsAnvil::Serialize::jsonExporter(reply.documents[loop])
                          << "\n\n";
            }
        }
    }
    // Check the Update works see if the filter finds it.
    {
        connection << send_Op_Update(fullConnection, {}, SimpleStringNoConstructor{"ThirdAndLast"}, StringAndIntNoConstructor{"Another", 45});

        connection << Op_Query<SimpleStringNoConstructor>(fullConnection, {.ret = 100}, "Another");
        StringAndIntNoConstructorReply                  replyData;
        auto                                            reply = get_Op_Reply(replyData);
        connection >> reply;

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(2, reply.numberReturned);
        if (!reply.isOk() || reply.numberReturned != 2)
        {
            std::cerr << "Showing: " << reply.numberReturned << "\n";
            for (int loop = 0; loop < reply.numberReturned; ++loop)
            {
                std::cerr << "Item: " << loop << "\n"
                          << ThorsAnvil::Serialize::jsonExporter(reply.documents[loop])
                          << "\n\n";
            }
        }
    }

    // Delete an item from the collection.
    {
        connection << send_Op_Delete(fullConnection, SimpleStringNoConstructor{"DataString"});
    }

    // Make sure that the delete worked.
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.ret = 100});
        StringAndIntNoConstructorReply                  replyData;
        auto                                            reply = get_Op_Reply(replyData);
        connection >> reply;

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(2, reply.numberReturned);
        if (!reply.isOk() || reply.numberReturned != 2)
        {
            std::cerr << "Showing: " << reply.numberReturned << "\n";
            for (int loop = 0; loop < reply.numberReturned; ++loop)
            {
                std::cerr << "Item: " << loop << "\n"
                          << ThorsAnvil::Serialize::jsonExporter(reply.documents[loop])
                          << "\n\n";
            }
        }
    }
}

class ConnectionTestMiddleWireAction: public UnitTestWithConnection
{
    public:
        ConnectionTestMiddleWireAction()
        {
            setCollectionToBaseLine(getConnection());
        };
};

/*
 * This test just verifies that Delete/Insert/Find work.
 *
 * Once we know this we can validate the commands with their own tests.
 * But we need to know these work so we can reset the DB to a clean state
 */
TEST(ConnectionTest, MiddleWireProtocol)
{
    using std::string_literals::operator""s;
    using namespace ThorsAnvil::DB::Mongo;

    MongoConnection  connection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});

    bool ok = true;
    ok = ok && ConnectionTestMiddleWireAction::zeroOutCollection(connection);
    ok = ok && ConnectionTestMiddleWireAction::checkTheNumberofObjectsIs("ConnectionTestMiddleWireAction:1", connection, 0);
    ok = ok && ConnectionTestMiddleWireAction::setTheDefaultCollectinState(connection);
    ok = ok && ConnectionTestMiddleWireAction::checkTheNumberofObjectsIs("ConnectionTestMiddleWireAction:2", connection, 5);

    ASSERT_TRUE(ok);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_Delete_2_Items)
{
    // Delete 2 item for the collection
    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_Delete("test", "ConnectionTest", FindValue{22});   // 22 matches 2 items

    std::size_t         count;
    CmdDB_DeleteReply   reply(count);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk())
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(2, count);   // number deleted

    checkTheNumberofObjectsIs("CmdDB_Delete_2_Items", ConnectionTestMiddleWireAction::getConnection(), 3);
}
TEST_F(ConnectionTestMiddleWireAction, CmdDB_Delete_2_Items_RValue)
{
    // Delete 2 item for the collection
    bool ok = false;
    std::size_t count;
    try
    {
        ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_Delete("test", "ConnectionTest", FindValue{22});   // 22 matches 2 items
        ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_DeleteReply(count);
        ok = true;
    }
    catch(MongoException const& e)
    {
        std::cerr << e
                  << "\n\n";
    }

    EXPECT_TRUE(true);

    checkTheNumberofObjectsIs("CmdDB_Delete_2_Items", ConnectionTestMiddleWireAction::getConnection(), 3);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindRemove_1_Items)
{
    // FindRemove 1 item for the collection
    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{48});   // 2 items have 48 values this should remove one
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

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

    checkTheNumberofObjectsIs("CmdDB_FindRemove_1_Items", ConnectionTestMiddleWireAction::getConnection(), 4);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindRemove_0_Items)
{
    // FindRemove Remove an item that is not there.
    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{112});
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr, result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindRemove_0_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_1_Items)
{
    // FindUpdate 1 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

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

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Items)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Item_And_Upsert)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert", ConnectionTestMiddleWireAction::getConnection(), 6);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_1_Items_ReturnUpdated)
{
    // FindUpdate 1 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(0,   result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);
    EXPECT_TRUE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items_ReturnUpdated", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Items_ReturnNull)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || result.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items_ReturnNull", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true,.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply(result);
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_NE(nullptr,   result.get());
    EXPECT_FALSE(reply.reply.lastErrorObject.updatedExisting);
    EXPECT_EQ(0, result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted", ConnectionTestMiddleWireAction::getConnection(), 6);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindRemove_1_Items_RValue)
{
    // FindRemove 1 item for the collection
    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{48});   // 2 items have 48 values this should remove one
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindDeleteReply(result);

    bool item1 = result->message == "ThisAndThat";
    bool item2 = result->message == "DataString";
    if (result.get() == nullptr || result->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(48,   result.get()->value);
    EXPECT_TRUE(item1 || item2);

    checkTheNumberofObjectsIs("CmdDB_FindRemove_1_Items", ConnectionTestMiddleWireAction::getConnection(), 4);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindRemove_0_Items_RValue)
{
    // FindRemove Remove an item that is not there.
    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindDelete("test", "ConnectionTest", FindValue{112});
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindDeleteReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr, result.get());

    checkTheNumberofObjectsIs("CmdDB_FindRemove_0_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_1_Items_RValue)
{
    // FindUpdate 1 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    bool item1 = result->message == "ThisAndThat";
    bool item2 = result->message == "DataString";
    if (result.get() == nullptr || result->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(48,   result->value);
    EXPECT_TRUE(item1 || item2);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Items_RValue)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr,   result.get());

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Item_And_Upsert_RValue)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr,   result.get());

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert", ConnectionTestMiddleWireAction::getConnection(), 6);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_1_Items_ReturnUpdated_RValue)
{
    // FindUpdate 1 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    ASSERT_NE(nullptr, result.get());
    EXPECT_EQ(0,   result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items_ReturnUpdated", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Items_ReturnNull_RValue)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() != nullptr)
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_EQ(nullptr,   result.get());

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items_ReturnNull", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted_RValue)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true,.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    std::unique_ptr<StringAndIntNoConstructor>         result;
    ConnectionTestMiddleWireAction::getConnection() >> get_CmdDB_FindUpdateReply(result);;

    if (result.get() == nullptr || result->value != 0 || result->message != "Bad Things Happen if you don't test")
    {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }

    EXPECT_NE(nullptr,   result.get());
    EXPECT_EQ(0, result->value);
    EXPECT_EQ("Bad Things Happen if you don't test", result->message);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted", ConnectionTestMiddleWireAction::getConnection(), 6);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_GetLastError_NoError)
{
    ConnectionTestMiddleWireAction::getConnection() << send_CmdDB_GetLastError("test");
    CmdDB_GetLastErrorReply   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.reply.err.get() != nullptr || reply.reply.n != 0)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   reply.reply.err.get());
    EXPECT_EQ(0,         reply.reply.n);

    checkTheNumberofObjectsIs("CmdDB_GetLastError_NoError", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_GetMore)
{
    MongoConnection& connection = ConnectionTestMiddleWireAction::getConnection();

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

TEST_F(ConnectionTestMiddleWireAction, CmdDB_GetMoreUsing_RValue)
{
    MongoConnection& connection = ConnectionTestMiddleWireAction::getConnection();

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

