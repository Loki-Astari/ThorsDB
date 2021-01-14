
#include <gtest/gtest.h>
#include "Op_Insert.h"
#include "Op_Delete.h"
#include "Op_Query.h"
#include "Op_Reply.h"
#include "Op_GetMore.h"
#include "Op_KillCursors.h"
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


TEST(ConnectionTest, SendToMongo)
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
        connection << Op_Query<FindAll>(fullConnection, QueryOptions{}, 100, 0);
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_EQ(0, reply.numberReturned);
        if (reply.numberReturned != 0)
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
        connection << Op_Query<FindAll>(fullConnection, QueryOptions{}, 100, 0);
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_EQ(3, reply.numberReturned);
        if (reply.numberReturned != 3)
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
        connection << Op_Query<FindAll>(fullConnection, QueryOptions{}, 2, 0);

        Op_Reply<StringAndIntNoConstructorReply>     reply1;
        connection >> reply1;
        std::cerr << make_hr(reply1);

        EXPECT_EQ(reply1.startingFrom, 0);
        EXPECT_EQ(reply1.numberReturned, 2);
        ASSERT_EQ(reply1.documents.size(), 2);
        EXPECT_EQ(reply1.documents[0].ok, 1.0);
        EXPECT_EQ(reply1.documents[1].ok, 1.0);

        connection << Op_GetMore(fullConnection, 1, reply1.cursorID);

        Op_Reply<StringAndIntNoConstructorReply>     reply2;
        connection >> reply2;
        EXPECT_EQ(reply2.startingFrom, 2);
        EXPECT_EQ(reply2.numberReturned, 1);
        ASSERT_EQ(reply2.documents.size(), 1);
        EXPECT_EQ(reply2.documents[0].ok, 1.0);

        connection << Op_GetMore(fullConnection, 1, reply1.cursorID);

        Op_Reply<StringAndIntNoConstructorReply>     reply3;
        connection >> reply3;
        EXPECT_EQ(reply3.startingFrom, 3);
        EXPECT_EQ(reply3.numberReturned, 0);
        ASSERT_GE(reply3.documents.size(), 0);
    }
    // Get the two of three objects then kill cursor
    {
        connection << Op_Query<FindAll>(fullConnection, QueryOptions{}, 2, 0);

        Op_Reply<StringAndIntNoConstructorReply>     reply1;
        connection >> reply1;
        std::cerr << make_hr(reply1);

        EXPECT_EQ(reply1.startingFrom, 0);
        EXPECT_EQ(reply1.numberReturned, 2);
        ASSERT_EQ(reply1.documents.size(), 2);
        EXPECT_EQ(reply1.documents[0].ok, 1.0);
        EXPECT_EQ(reply1.documents[1].ok, 1.0);

        connection << Op_KillCursors({reply1.cursorID});
    }

    // Check the query can actual filter.
    {
        connection << Op_Query<SimpleStringNoConstructor>(fullConnection, QueryOptions{}, 100, 0, "Another");
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_EQ(1, reply.numberReturned);
        if (reply.numberReturned != 1)
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
        connection << Op_Query<FindAll>(fullConnection, QueryOptions{}, 100, 0);
        Op_Reply<StringAndIntNoConstructorReply>     reply;
        connection >> reply;

        EXPECT_EQ(2, reply.numberReturned);
        if (reply.numberReturned != 2)
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

