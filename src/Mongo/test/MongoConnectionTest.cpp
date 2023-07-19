#include "test/pipe.h"
#include <gtest/gtest.h>
#include "MongoConnection.h"
#include "MongoConfig.h"
#include "ThorsLogging/ThorsLogging.h"

TEST(MongoConnectionTest, CreateThorConnection)
{
    SocketSetUp         setupSocket;

    auto action = [](){
        using std::string_literals::operator""s;
        using namespace ThorsAnvil::DB::Access;
        std::string connectionString = "mongo://"s + THOR_TESTING_MONGO_HOST;
        Connection  connection(connectionString, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});
    };
}

TEST(MongoConnectionTest, CreateConnection)
{
    SocketSetUp         setupSocket;

    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        MongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});
    };
    EXPECT_NO_THROW(action());
}

TEST(MongoConnectionTest, CreateConnectionWithSnappy)
{
    SocketSetUp         setupSocket;

    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        MongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {{"compressors", "snappy"}});
    };
    EXPECT_NO_THROW(action());
}

TEST(MongoConnectionTest, CreateConnectionAppNameSet)
{
    SocketSetUp         setupSocket;

    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        MongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {{"AppName", "UnitTest"}});
    };
    EXPECT_NO_THROW(action());
}

TEST(MongoConnectionTest, CreateConnectionBadUser)
{
    SocketSetUp         setupSocket;

    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        MongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, "Bad", THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});
    };
    EXPECT_THROW(action(), ThorsAnvil::Logging::CriticalException);
}

TEST(MongoConnectionTest, CreateConnectionBadPass)
{
    SocketSetUp         setupSocket;

    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        MongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, "Bad", THOR_TESTING_MONGO_DB, {});
    };
    EXPECT_THROW(action(), ThorsAnvil::Logging::CriticalException);
}

TEST(MongoConnectionTest, CreateConnectionBadDB)
{
    SocketSetUp         setupSocket;

    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        MongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, "Bad", {});
    };
    EXPECT_THROW(action(), ThorsAnvil::Logging::CriticalException);
}

