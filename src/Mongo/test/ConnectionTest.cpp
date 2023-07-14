
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
    GTEST_SKIP() << "No longer supported\n";
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
