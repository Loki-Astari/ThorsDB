
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
#include "CmdDB_Insert.h"
#include "CmdDB_Delete.h"
// Other Stuff
#include "MongoConnection.h"
#include "MongoConfig.h"
#include "test/OpTest.h"

#include <iostream>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

struct FindAll {};
ThorsAnvil_MakeTrait(FindAll);

struct StringAndIntNoConstructorReply: public StringAndIntNoConstructor
{
    std::string         $err;
    std::int32_t        code;
    double              ok      = 1.0;
};

ThorsAnvil_ExpandTrait(StringAndIntNoConstructor, StringAndIntNoConstructorReply, $err, code, ok);


TEST(ConnectionTest, YeOldWireProtocol)
{
    using std::string_literals::operator""s;
    using namespace ThorsAnvil::DB::Mongo;

    MongoConnection  connection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});

    std::string fullConnection  = THOR_TESTING_MONGO_DB;
    fullConnection += ".ConnectionTest";

    // reset the collection to be empty.
    {
        connection << Op_Delete<FindAll>(fullConnection);
    }

    // Make sure there are zero objects in the collection.
    {
        connection << Op_Query<FindAll>(fullConnection, {.skip = 100, .ret = 0});
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(0, reply.numberReturned);
        if (!reply.isOk() || reply.numberReturned != 0)
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

    // Add three objects to the collection
    {
        StringAndIntNoConstructor               object1{"DataString"s, 48};
        StringAndIntNoConstructor               object2{"Another"s, 22};
        StringAndIntNoConstructor               object3{"ThirdAndLast"s, 0xFF};

        connection << Op_Insert<StringAndIntNoConstructor>(fullConnection, object1, object2, object3);
    }

    // Check there are three objects in the collection.
    {
        connection << Op_Query<FindAll>(fullConnection, {.ret = 100});
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(3, reply.numberReturned);
        if (!reply.isOk() || reply.numberReturned != 3)
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

    // Get the three objects one at time using GET_MORE
    {
        connection << Op_Query<FindAll>(fullConnection, {.ret = 2});

        Op_Reply<StringAndIntNoConstructorReply>     reply1;
        connection >> reply1;

        EXPECT_TRUE(reply1.isOk());
        EXPECT_EQ(reply1.startingFrom, 0);
        EXPECT_EQ(reply1.numberReturned, 2);
        ASSERT_EQ(reply1.documents.size(), 2);

        connection << Op_GetMore(fullConnection, 1, reply1.cursorID);

        Op_Reply<StringAndIntNoConstructorReply>     reply2;
        connection >> reply2;
        EXPECT_TRUE(reply2.isOk());
        EXPECT_EQ(reply2.startingFrom, 2);
        EXPECT_EQ(reply2.numberReturned, 1);
        ASSERT_EQ(reply2.documents.size(), 1);

        connection << Op_GetMore(fullConnection, 1, reply1.cursorID);

        Op_Reply<StringAndIntNoConstructorReply>     reply3;
        connection >> reply3;
        EXPECT_TRUE(reply3.isOk());
        EXPECT_EQ(reply3.startingFrom, 3);
        EXPECT_EQ(reply3.numberReturned, 0);
        ASSERT_GE(reply3.documents.size(), 0);
    }
    // Get the two of three objects then kill cursor
    {
        connection << Op_Query<FindAll>(fullConnection, {.ret = 2});

        Op_Reply<StringAndIntNoConstructorReply>     reply1;
        connection >> reply1;

        EXPECT_TRUE(reply1.isOk());
        EXPECT_EQ(reply1.startingFrom, 0);
        EXPECT_EQ(reply1.numberReturned, 2);
        ASSERT_EQ(reply1.documents.size(), 2);

        connection << Op_KillCursors({reply1.cursorID});

        connection << Op_GetMore(fullConnection, 1, reply1.cursorID);

        Op_Reply<StringAndIntNoConstructorReply>     reply2;
        connection >> reply2;
        EXPECT_FALSE(reply2.isOk());
        EXPECT_EQ(reply2.responseFlags & OP_ReplyFlag::CursorNotFound, OP_ReplyFlag::CursorNotFound);
    }

    // Check the query can actual filter.
    {
        connection << Op_Query<SimpleStringNoConstructor>(fullConnection, {.ret = 100}, "Another");
        Op_Reply<StringAndIntNoConstructorReply>     reply;
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
        connection << Op_Update<SimpleStringNoConstructor, StringAndIntNoConstructor>(fullConnection, SimpleStringNoConstructor{"ThirdAndLast"}, StringAndIntNoConstructor{"Another", 45});

        connection << Op_Query<SimpleStringNoConstructor>(fullConnection, {.ret = 100}, "Another");
        Op_Reply<StringAndIntNoConstructorReply>     reply;
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
        connection << Op_Delete<SimpleStringNoConstructor>(fullConnection, "DataString");
    }

    // Make sure that the delete worked.
    {
        connection << Op_Query<FindAll>(fullConnection, {.ret = 100});
        Op_Reply<StringAndIntNoConstructorReply>     reply;
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
        connection << make_CmdDB_Delete("test", "ConnectionTest", {}, FindAll{});
        CmdDB_Reply   reply;
        connection >> reply;

        if (!reply.isOk() || reply.getDocumentCount() != 1 || reply.getDocument(0).ok != 1.0)
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.getDocumentCount());
        EXPECT_EQ(1.0, reply.getDocument(0).ok);
    }

    // Make sure there are zero objects in the collection.
    {
        connection << Op_Query<FindAll>(fullConnection, {.ret = 100});
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(0, reply.numberReturned);
        if (!reply.isOk() || reply.numberReturned != 0)
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

    // Add three objects to the collection
    {
        std::vector<StringAndIntNoConstructor>               objects{StringAndIntNoConstructor{"DataString"s, 48},
                                                                     StringAndIntNoConstructor{"Another"s, 22},
                                                                     StringAndIntNoConstructor{"ThirdAndLast"s, 0xFF}
                                                                    };

        connection << make_CmdDB_Insert("test", "ConnectionTest", {}, std::begin(objects), std::end(objects));
        CmdDB_Reply   reply;
        connection >> reply;

        if (!reply.isOk() || reply.getDocument(0).n != 3)
        {
            std::cerr << make_hr(reply);
        }

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(1,   reply.getDocumentCount());
        EXPECT_EQ(1.0, reply.getDocument(0).ok);
        EXPECT_EQ(3,   reply.getDocument(0).n);
    }

    // Check there are three objects in the collection.
    {
        connection << Op_Query<FindAll>(fullConnection, {.ret = 100});
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_TRUE(reply.isOk());
        EXPECT_EQ(3, reply.numberReturned);
        if (!reply.isOk() || reply.numberReturned != 3)
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


