
#include <gtest/gtest.h>
#include "Op_Insert.h"
#include "Op_Delete.h"
#include "Op_Query.h"
#include "Op_Reply.h"
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
    double              ok;
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

