
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
        // connection << Op_Delete<FindAll>(fullConnection);
        CmdDB_Delete<FindAll>   deleter{"test", "ConnectionTest", QueryOptions{}, DeleteQuery<FindAll>{FindAll{},0/*, Collation{}, ""*/}, false, WriteConcern{}, "Delete Comment"};

        connection << CmdDB_Delete<FindAll>{"test", "ConnectionTest", QueryOptions{}, DeleteQuery<FindAll>{FindAll{},0/*, Collation{}, ""*/}, false, WriteConcern{}, "Delete Comment"};
        CmdDB_Reply   reply;
        connection >> reply;
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

        // connection << Op_Insert<StringAndIntNoConstructor>(fullConnection, object1, object2, object3);
        std::cerr << make_hr(CmdDB_Insert<StringAndIntNoConstructor>{"test", "ConnectionTest", QueryOptions{}, std::begin(objects), std::end(objects), false, WriteConcern{}, false, "Comment"});
        connection << CmdDB_Insert<StringAndIntNoConstructor>{"test", "ConnectionTest", QueryOptions{}, std::begin(objects), std::end(objects), false, WriteConcern{}, false, "Comment"};
        CmdDB_Reply   reply;
        connection >> reply;
        std::cerr << make_hr(reply);
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

