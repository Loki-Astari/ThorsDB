
#include <gtest/gtest.h>
#include "MongoConnection.h"
#include "CmdDB_Insert.h"
#include "CmdDB_Delete.h"
#include "MongoConfig.h"
#include "OpTest.h"
#include "Op_Delete.h"

#include <iostream>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

struct FindAll {};
ThorsAnvil_MakeTrait(FindAll);

struct StringAndIntNoConstructorReplyTwo: public StringAndIntNoConstructor
{
    std::string         $err;
    std::int32_t        code;
    double              ok      = 1.0;
};
ThorsAnvil_ExpandTrait(StringAndIntNoConstructor, StringAndIntNoConstructorReplyTwo, $err, code, ok);

TEST(CmdDB_InsertTest, SendToMongo)
{
    using std::string_literals::operator""s;
    using namespace ThorsAnvil::DB::Mongo;

    MongoConnection  connection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});

    std::string fullConnection  = THOR_TESTING_MONGO_DB;
    fullConnection += ".ConnectionTest";

    // reset the collection to be empty.
    {
        connection << make_CmdDB_Delete("test", "ConnectionTest", QueryOptions{}, FindAll{});
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
        connection << Op_Query<FindAll>(fullConnection, QueryOptions{}, 100, 0);
        Op_Reply<StringAndIntNoConstructorReplyTwo>     reply;
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

        connection << make_CmdDB_Insert("test", "ConnectionTest", QueryOptions{}, std::begin(objects), std::end(objects));
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
        connection << Op_Query<FindAll>(fullConnection, QueryOptions{}, 100, 0);
        Op_Reply<StringAndIntNoConstructorReplyTwo>     reply;
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

