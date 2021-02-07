
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


TEST(ConnectionTest, MiddleWireProtocol)
{
    using std::string_literals::operator""s;
    using namespace ThorsAnvil::DB::Mongo;

    MongoConnection  connection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});

    std::string fullConnection  = THOR_TESTING_MONGO_DB;
    fullConnection += ".ConnectionTest";

    // reset the collection to be empty.
    {
        connection << make_CmdDB_Delete("test", "ConnectionTest", TestFindAll{});
        CmdDB_DeleteReply   reply;
        connection >> reply;

        if (!reply.isOk() || reply.reply.size() != 1 || reply.reply[0].ok != 1.0)
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.reply.size());
        EXPECT_EQ(1.0, reply.reply[0].ok);
    }

    // Make sure there are zero objects in the collection.
    {
        //connection << Op_Query<TestFindAll>(fullConnection, {.ret = 100});
        connection << make_CmdDB_Find("test", "ConnectionTest");

        // Op_Reply<StringAndIntNoConstructorReply>     reply;
        // connection >> reply;
        CmdDB_Reply     reply;
        connection >> reply;

        if (!reply.isOk() || reply.reply.size() != 1 || reply.reply[0].ok != 1.0)
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.reply.size());
        EXPECT_EQ(1.0, reply.reply[0].ok);
        EXPECT_EQ(0,   reply.replyCount());
    }

    // Add five objects to the collection
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

        if (!reply.isOk() || reply.reply.size() == 0 || reply.reply[0].n != 5)
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.reply.size());
        EXPECT_EQ(1.0, reply.reply[0].ok);
        EXPECT_EQ(5,   reply.reply[0].n);
    }

    // Check there are five objects in the collection.
    {
        auto find = make_CmdDB_Find("test", "ConnectionTest");;
        connection << find;

        CmdDB_FindReply<StringAndIntNoConstructor> reply;
        connection >> reply;

        if (!reply.isOk() || reply.numberReturned != 1 || reply.findData.cursor.firstBatch.size() != 5)
        {
            std::cerr << make_hr(reply);
        }
        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.numberReturned);
        EXPECT_EQ(1.0, reply.findData.ok);
        EXPECT_EQ(5,   reply.findData.cursor.firstBatch.size());
    }
    // Delete 2 item for the collection
    {
        connection << make_CmdDB_Delete("test", "ConnectionTest", FindValue{22});   // 22 matches 2 items
        CmdDB_DeleteReply   reply;
        connection >> reply;

        if (!reply.isOk() || reply.reply.size() != 1 || reply.reply[0].ok != 1.0)
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.reply.size());
        EXPECT_EQ(1.0, reply.reply[0].ok);
        EXPECT_EQ(2,   reply.reply[0].n);   // number deleted
    }
    // Check there are three objects in the collection.
    {
        auto find = make_CmdDB_Find("test", "ConnectionTest");;
        connection << find;

        CmdDB_FindReply<StringAndIntNoConstructor> reply;
        connection >> reply;

        if (!reply.isOk() || reply.numberReturned != 1 || reply.findData.cursor.firstBatch.size() != 3)
        {
            std::cerr << make_hr(reply);
        }
        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.numberReturned);
        EXPECT_EQ(1.0, reply.findData.ok);
        EXPECT_EQ(3,   reply.findData.cursor.firstBatch.size());
    }
    // FindRemove 1 item for the collection
    {
        connection << make_CmdDB_FindDelete("test", "ConnectionTest", {}, FindValue{48});   // 2 items have 48 values this should remove one
        CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
        connection >> reply;

        bool item1 = reply.findData.value->message == "ThisAndThat";
        bool item2 = reply.findData.value->message == "DataString";
        if (!reply.isOk() || reply.findData.ok != 1.0 || reply.findData.value->value != 48 || ((item1 || item2) != true))
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1.0,  reply.findData.ok);
        EXPECT_EQ(48,   reply.findData.value->value);
        EXPECT_TRUE(item1 || item2);
        EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);
    }
    // Check there are two objects in the collection.
    {
        auto find = make_CmdDB_Find("test", "ConnectionTest");;
        connection << find;

        CmdDB_FindReply<StringAndIntNoConstructor> reply;
        connection >> reply;

        if (!reply.isOk() || reply.numberReturned != 1 || reply.findData.cursor.firstBatch.size() != 2)
        {
            std::cerr << make_hr(reply);
        }
        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.numberReturned);
        EXPECT_EQ(1.0, reply.findData.ok);
        EXPECT_EQ(2,   reply.findData.cursor.firstBatch.size());
    }
    // FindRemove Remove an item that is not there.
    {
        connection << make_CmdDB_FindDelete("test", "ConnectionTest", {}, FindValue{112});
        CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
        connection >> reply;

        if (!reply.isOk() || reply.findData.ok != 1.0 || reply.findData.value.get() != nullptr)
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1.0, reply.findData.ok);
        EXPECT_EQ(nullptr,  reply.findData.value.get());
        EXPECT_FALSE(reply.findData.lastErrorObject.updatedExisting);
    }
    // Check there are two objects in the collection.
    {
        auto find = make_CmdDB_Find("test", "ConnectionTest");;
        connection << find;

        CmdDB_FindReply<StringAndIntNoConstructor> reply;
        connection >> reply;

        if (!reply.isOk() || reply.numberReturned != 1 || reply.findData.cursor.firstBatch.size() != 2)
        {
            std::cerr << make_hr(reply);
        }
        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.numberReturned);
        EXPECT_EQ(1.0, reply.findData.ok);
        EXPECT_EQ(2,   reply.findData.cursor.firstBatch.size());
    }
    // FindUpdate 1 item for the collection
    {
        connection << make_CmdDB_FindUpdate("test", "ConnectionTest", {}, UpdateMessage{"Bad Things Happen if you don't test"}, FindValue{48});   // 1 items has 48 value Update the message.
        CmdDB_FindModifyReply<StringAndIntNoConstructor>   reply;
        connection >> reply;

        bool item1 = reply.findData.value->message == "ThisAndThat";
        bool item2 = reply.findData.value->message == "DataString";
        if (!reply.isOk() || reply.findData.ok != 1.0 || reply.findData.value->value != 48 || ((item1 || item2) != true))
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1.0,  reply.findData.ok);
        EXPECT_EQ(48,   reply.findData.value->value);
        EXPECT_TRUE(item1 || item2);
        EXPECT_TRUE(reply.findData.lastErrorObject.updatedExisting);
    }
}


