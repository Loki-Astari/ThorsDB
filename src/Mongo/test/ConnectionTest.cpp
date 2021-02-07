
#include <gtest/gtest.h>
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

struct TestFindAll {};
ThorsAnvil_MakeTrait(TestFindAll);

struct StringAndIntNoConstructorReply: public StringAndIntNoConstructor
{
    std::string         $err    = "Not Initialized";
    std::int32_t        code    = 404;
    double              ok      = 1.0;
};
struct FindValue
{
    int value;
};
struct UpdateMessage
{
    std::string message;
};

ThorsAnvil_ExpandTrait(StringAndIntNoConstructor, StringAndIntNoConstructorReply, $err, code, ok);
ThorsAnvil_MakeTrait(FindValue, value);
ThorsAnvil_MakeTrait(UpdateMessage, message);


TEST(ConnectionTest, YeOldWireProtocol)
{
    using std::string_literals::operator""s;
    using namespace ThorsAnvil::DB::Mongo;

    MongoConnection  connection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});
    ErrorInfo        info;
    bool             error = true;

    std::string fullConnection  = THOR_TESTING_MONGO_DB;
    fullConnection += ".ConnectionTest";

    // reset the collection to be empty.
    {
        connection << make_Op_Delete(fullConnection, TestFindAll{});
    }

    // Make sure there are zero objects in the collection.
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.skip = 100, .ret = 0});
        std::vector<StringAndIntNoConstructorReply>   replyData;

        try
        {
            connection >> make_Op_Reply(replyData);
            EXPECT_EQ(0, replyData.size());
            error = false;
        }
        catch(MongoException const& e)
        {
            info = e.info;
        }

        if (error)
        {
            std::cerr << info.getHRErrorMessage()
                      << ThorsAnvil::Serialize::jsonExporter(replyData)
                      << "\n\n";
        }
    }

    // Add three objects to the collection
    {
        std::vector<StringAndIntNoConstructor>  data({{"DataString"s, 48},
                                                      {"Another"s, 22},
                                                      {"ThirdAndLast"s, 0xFF}});

        connection << make_Op_Insert(fullConnection, data);
    }

    // Check there are three objects in the collection.
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.ret = 100});
        std::vector<StringAndIntNoConstructorReply> replyData;
        error = true;

        try
        {
            connection >> make_Op_Reply(replyData);
            EXPECT_EQ(3, replyData.size());
            error = false;
        }
        catch(MongoException const& e)
        {
            info = e.info;
        }

        if (error)
        {
            std::cerr << info.getHRErrorMessage()
                      << ThorsAnvil::Serialize::jsonExporter(replyData)
                      << "\n\n";
        }
    }

    // Get the three objects one at time using GET_MORE
    {
        std::vector<StringAndIntNoConstructorReply>             replyData1;
        std::vector<StringAndIntNoConstructorReply>             replyData2;
        std::vector<StringAndIntNoConstructorReply>             replyData3;

        error = true;
        try
        {
            connection << Op_Query<TestFindAll>(fullConnection, {.ret = 2});

            connection >> make_Op_Reply(replyData1);
            EXPECT_EQ(2, replyData1.size());
            auto cursorInfo1 = connection.getCursorInfo(connection.getLastOpenCursor());
            EXPECT_EQ(cursorInfo1.first, 0);
            EXPECT_EQ(cursorInfo1.second, 2);

            connection << make_Op_GetMore(fullConnection, 1);

            connection >> make_Op_Reply(replyData2);
            EXPECT_EQ(1, replyData2.size());
            auto cursorInfo2 = connection.getCursorInfo(connection.getLastOpenCursor());
            EXPECT_EQ(cursorInfo2.first, 0);
            EXPECT_EQ(cursorInfo2.second, 3);

            connection << make_Op_GetMore(fullConnection, 1);

            connection >> make_Op_Reply(replyData3);
            EXPECT_EQ(0, replyData3.size());
            auto cursorInfo3 = connection.getCursorInfo(connection.getLastOpenCursor());
            EXPECT_EQ(cursorInfo3.first, 0);
            EXPECT_EQ(cursorInfo3.second, 3);
            error = false;
        }
        catch(MongoException const& e)
        {
            info = e.info;
        }

        if (error)
        {
            std::cerr << info.getHRErrorMessage()
                      << ThorsAnvil::Serialize::jsonExporter(replyData1)
                      << ThorsAnvil::Serialize::jsonExporter(replyData2)
                      << ThorsAnvil::Serialize::jsonExporter(replyData3)
                      << "\n\n";
        }
    }
    // Get the two of three objects then kill cursor
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.ret = 2});

        std::vector<StringAndIntNoConstructorReply>             replyData1;
        Op_Reply<std::vector<StringAndIntNoConstructorReply>>   reply1(replyData1);
        connection >> reply1;

        EXPECT_TRUE(reply1.isOk());
        EXPECT_EQ(reply1.startingFrom, 0);
        EXPECT_EQ(reply1.numberReturned, 2);
        ASSERT_EQ(reply1.documents.size(), 2);

        connection << make_Op_KillCursors(reply1);

        connection << make_Op_GetMore(fullConnection, reply1);

        StringAndIntNoConstructorReply              replyData2;
        Op_Reply<StringAndIntNoConstructorReply>    reply2(replyData2);
        connection >> reply2;
        EXPECT_FALSE(reply2.isOk());
        EXPECT_EQ(reply2.responseFlags & OP_ReplyFlag::CursorNotFound, OP_ReplyFlag::CursorNotFound);
    }

    // Check the query can actual filter.
    {
        connection << Op_Query<SimpleStringNoConstructor>(fullConnection, {.ret = 100}, "Another");
        StringAndIntNoConstructorReply              replyData;
        Op_Reply<StringAndIntNoConstructorReply>    reply(replyData);
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
        connection << make_Op_Update(fullConnection, {}, SimpleStringNoConstructor{"ThirdAndLast"}, StringAndIntNoConstructor{"Another", 45});

        connection << Op_Query<SimpleStringNoConstructor>(fullConnection, {.ret = 100}, "Another");
        StringAndIntNoConstructorReply              replyData;
        Op_Reply<StringAndIntNoConstructorReply>    reply(replyData);
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
        connection << make_Op_Delete(fullConnection, SimpleStringNoConstructor{"DataString"});
    }

    // Make sure that the delete worked.
    {
        connection << Op_Query<TestFindAll>(fullConnection, {.ret = 100});
        StringAndIntNoConstructorReply              replyData;
        Op_Reply<StringAndIntNoConstructorReply>    reply(replyData);
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

static bool checkTheNumberofObjectsIs(std::string const& message, MongoConnection& connection, int expected)
{
    connection << make_CmdDB_Find("test", "ConnectionTest");

    CmdDB_FindReply<StringAndIntNoConstructor>     reply;
    connection >> reply;

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(expected,   reply.findData.cursor.firstBatch.size());
    EXPECT_EQ(1.0, reply.findData.ok);

    if (!reply.isOk() || reply.findData.cursor.firstBatch.size() != expected || reply.findData.ok != 1.0)
    {
        std::cerr << "Fail in: " << message << "\n";
        std::cerr << make_hr(reply);
        return false;
    }
    return true;
}
static bool zeroOutCollection(MongoConnection& connection)
{
    connection << make_CmdDB_Delete("test", "ConnectionTest", TestFindAll{});

    CmdDB_DeleteReply   reply;
    connection >> reply;

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(1,   reply.reply.size());

    if (!reply.isOk() || reply.reply.size() != 1)
    {
        std::cerr << make_hr(reply);
        return false;
    }
    return true;
}
static bool setTheDefaultCollectinState(MongoConnection& connection)
{
    std::vector<StringAndIntNoConstructor>               objects{{"DataString"s, 48},
                                                                 {"Another"s, 22},
                                                                 {"ThirdAndLast"s, 0xFF},
                                                                 {"ThisAndThat", 48},
                                                                 {"Bit The Dust", 22},
                                                                };

    connection << make_CmdDB_Insert("test", "ConnectionTest", std::begin(objects), std::end(objects));
    CmdDB_InsertReply   reply;
    connection >> reply;

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(1,   reply.reply.size());
    EXPECT_EQ(5,   reply.reply[0].n);

    if (!reply.isOk() || reply.reply.size() == 0 || reply.reply[0].n != 5)
    {
        std::cerr << make_hr(reply);
        return false;
    }
    return true;
}

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
    ok = ok && zeroOutCollection(connection);
    ok = ok && checkTheNumberofObjectsIs("MiddleWireProtocol:1", connection, 0);
    ok = ok && setTheDefaultCollectinState(connection);
    ok = ok && checkTheNumberofObjectsIs("MiddleWireProtocol:2", connection, 5);

    ASSERT_TRUE(ok);
}

class ConnectionTestMiddleWireAction : public ::testing::Test
{
    public:
        ConnectionTestMiddleWireAction()
        {
            setCollectionToBaseLine(getConnection());
        }
        static void SetUpTestCase()
        {
            connection.reset(new MongoConnection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {}));
        }
        static void TearDownTestCase()
        {
            connection.reset();
        }

        static MongoConnection& getConnection() {return *connection;}
    private:
        static std::unique_ptr<MongoConnection> connection;
        static void setCollectionToBaseLine(MongoConnection& connection)
        {
            bool ok = true;
            ok = ok && zeroOutCollection(connection);
            ok = ok &&  checkTheNumberofObjectsIs("setCollectionToBaseLine:1", connection, 0);
            ok = ok &&  setTheDefaultCollectinState(connection);
            ok = ok &&  checkTheNumberofObjectsIs("setCollectionToBaseLine:2", connection, 5);

            // If this fails.
            // Make sure the "MiddleWireProtocol" is working correctly.
            ASSERT_TRUE(ok);
        }
};
std::unique_ptr<MongoConnection> ConnectionTestMiddleWireAction::connection;

TEST_F(ConnectionTestMiddleWireAction, CmdDB_Delete_2_Items)
{
    // Delete 2 item for the collection
    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_Delete("test", "ConnectionTest", FindValue{22});   // 22 matches 2 items
    CmdDB_DeleteReply   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.reply.size() != 1)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(1,   reply.reply.size());
    EXPECT_EQ(2,   reply.reply[0].n);   // number deleted

    checkTheNumberofObjectsIs("CmdDB_Delete_2_Items", ConnectionTestMiddleWireAction::getConnection(), 3);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindRemove_1_Items)
{
    // FindRemove 1 item for the collection
    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindDelete("test", "ConnectionTest", {}, FindValue{48});   // 2 items have 48 values this should remove one
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    bool item1 = reply.findData.value->message == "ThisAndThat";
    bool item2 = reply.findData.value->message == "DataString";
    if (!reply.isOk() || reply.findData.value.get() == nullptr || reply.findData.value->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    ASSERT_NE(nullptr, reply.findData.value.get());
    EXPECT_EQ(48,   reply.findData.value->value);
    EXPECT_TRUE(item1 || item2);
    EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindRemove_1_Items", ConnectionTestMiddleWireAction::getConnection(), 4);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindRemove_0_Items)
{
    // FindRemove Remove an item that is not there.
    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindDelete("test", "ConnectionTest", {}, FindValue{112});
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.findData.value.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,  reply.findData.value.get());
    EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindRemove_0_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_1_Items)
{
    // FindUpdate 1 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindUpdate("test", "ConnectionTest", {}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    bool item1 = reply.findData.value->message == "ThisAndThat";
    bool item2 = reply.findData.value->message == "DataString";
    if (!reply.isOk() || reply.findData.value.get() == nullptr || reply.findData.value->value != 48 || ((item1 || item2) != true))
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    ASSERT_NE(nullptr, reply.findData.value.get());
    EXPECT_EQ(48,   reply.findData.value->value);
    EXPECT_TRUE(item1 || item2);
    EXPECT_TRUE(reply.findData.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Items)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindUpdate("test", "ConnectionTest", {}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.findData.value.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   reply.findData.value.get());
    EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Item_And_Upsert)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindUpdate("test", "ConnectionTest", {.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.findData.value.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   reply.findData.value.get());
    EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert", ConnectionTestMiddleWireAction::getConnection(), 6);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_1_Items_ReturnUpdated)
{
    // FindUpdate 1 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.findData.value.get() == nullptr || reply.findData.value->value != 0 || reply.findData.value->message != "Bad Things Happen if you don't test")
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    ASSERT_NE(nullptr, reply.findData.value.get());
    EXPECT_EQ(0,   reply.findData.value->value);
    EXPECT_EQ("Bad Things Happen if you don't test", reply.findData.value->message);
    EXPECT_TRUE(reply.findData.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_1_Items_ReturnUpdated", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Items_ReturnNull)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.findData.value.get() != nullptr)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   reply.findData.value.get());
    EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Items_ReturnNull", ConnectionTestMiddleWireAction::getConnection(), 5);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted)
{
    // FindUpdate 0 item for the collection

    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_FindUpdate("test", "ConnectionTest", {.returnModified=true,.upsert=true}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{66});   // 1 items has 48 value Update the message.
    CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;

    if (!reply.isOk() || reply.findData.value.get() == nullptr || reply.findData.value->value != 0 || reply.findData.value->message != "Bad Things Happen if you don't test")
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_NE(nullptr,   reply.findData.value.get());
    EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);
    EXPECT_EQ(0, reply.findData.value->value);
    EXPECT_EQ("Bad Things Happen if you don't test", reply.findData.value->message);

    checkTheNumberofObjectsIs("CmdDB_FindUpdate_Miss_Item_And_Upsert_ReturnUpserted", ConnectionTestMiddleWireAction::getConnection(), 6);
}

TEST_F(ConnectionTestMiddleWireAction, CmdDB_GetLastError_NoError)
{
    std::cerr << "\n\nSend Start\n\n";
    ConnectionTestMiddleWireAction::getConnection() << make_CmdDB_GetLastError("test");
    std::cerr << "\n\nSend Done\n\n";
    CmdDB_GetLastErrorReply   reply;
    ConnectionTestMiddleWireAction::getConnection() >> reply;
    std::cerr << "\n\nReceive Done\n\n";

    if (!reply.isOk() || reply.lastReply.err.get() == nullptr || reply.lastReply.n != 0)
    {
        std::cerr << make_hr(reply);
    }

    EXPECT_TRUE(reply.isOk());
    EXPECT_EQ(nullptr,   reply.lastReply.err.get());
    EXPECT_EQ(0,         reply.lastReply.n);

    checkTheNumberofObjectsIs("CmdDB_GetLastError_NoError", ConnectionTestMiddleWireAction::getConnection(), 5);
}

