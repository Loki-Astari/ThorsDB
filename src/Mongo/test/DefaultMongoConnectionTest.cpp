
#include <gtest/gtest.h>
#include "DefaultMongoConnection.h"
#include "MongoConfig.h"
#include "ThorsLogging/ThorsLogging.h"

TEST(DefaultMongoConnectionTest, CreateThorConnection)
{
    auto action = [](){
        using std::string_literals::operator""s;
        using namespace ThorsAnvil::DB::Access;
        std::string connectionString = "mongo://"s + THOR_TESTING_MONGO_HOST;
        Connection  connection(connectionString, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});
    };
}

TEST(DefaultMongoConnectionTest, CreateConnection)
{
    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        DefaultMongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});
    };
    EXPECT_NO_THROW(action());
}
TEST(DefaultMongoConnectionTest, CreateConnectionAppNameSet)
{
    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        DefaultMongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {{"AppName", "UnitTest"}});
    };
    EXPECT_NO_THROW(action());
}

TEST(DefaultMongoConnectionTest, CreateConnectionBadUser)
{
    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        DefaultMongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, "Bad", THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {});
    };
    EXPECT_THROW(action(), ThorsAnvil::Logging::CriticalException);
}

TEST(DefaultMongoConnectionTest, CreateConnectionBadPass)
{
    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        DefaultMongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, "Bad", THOR_TESTING_MONGO_DB, {});
    };
    EXPECT_THROW(action(), ThorsAnvil::Logging::CriticalException);
}

TEST(DefaultMongoConnectionTest, CreateConnectionBadDB)
{
    auto action = [](){
        using namespace ThorsAnvil::DB::Mongo;
        DefaultMongoConnection  connection(THOR_TESTING_MONGO_HOST, 0, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, "Bad", {});
    };
    EXPECT_THROW(action(), ThorsAnvil::Logging::CriticalException);
}

