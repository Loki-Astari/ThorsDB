#include "test/UnitTestWithConnection.h"
#include "MongoConfig.h"
// Middle Wire Protocol
#include "CmdDB_Reply.h"
#include "CmdDB_Insert.h"
#include "CmdDB_Delete.h"
#include "CmdDB_Find.h"

#include <vector>

using namespace ThorsAnvil::DB::Mongo;
using std::string_literals::operator""s;

std::unique_ptr<MongoConnection> UnitTestWithConnection::connection;

#if 0
UnitTestWithConnection::UnitTestWithConnection()
{
    setCollectionToBaseLine(getConnection());
}
#endif

void UnitTestWithConnection::SetUpTestCase()
{
    connection.reset(new MongoConnection(THOR_TESTING_MONGO_HOST, 27017, THOR_TESTING_MONGO_USER, THOR_TESTING_MONGO_PASS, THOR_TESTING_MONGO_DB, {}));
}

void UnitTestWithConnection::TearDownTestCase()
{
    connection.reset();
}

MongoConnection& UnitTestWithConnection::getConnection()
{
    return *connection;
}

void UnitTestWithConnection::setCollectionToBaseLine(MongoConnection& connection)
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

bool UnitTestWithConnection::checkTheNumberofObjectsIs(std::string const& message, MongoConnection& connection, int expected)
{
    std::vector<StringAndIntNoConstructor>      data;

    try
    {
        connection << send_CmdDB_Find("test", "ConnectionTest");
        connection >> get_CmdDB_FindReply(data);
    }
    catch(MongoException const& e)
    {
        EXPECT_TRUE(false);
        std::cerr << "Fail in: " << message
                  << " Exception: " << e
                  << "\n\n";
        return false;
    }

    EXPECT_EQ(expected,   data.size());

    if (data.size() != expected)
    {
        std::cerr << "Fail in: " << message
                  << "Bad Result Size\n";
        return false;
    }
    return true;
}

bool UnitTestWithConnection::zeroOutCollection(MongoConnection& connection)
{
    connection << send_CmdDB_Delete("test", "ConnectionTest", TestFindAll{});

    std::size_t     size;
    CmdDB_DeleteReply   reply(size);
    connection >> reply;

    EXPECT_TRUE(reply.isOk());

    if (!reply.isOk())
    {
        std::cerr << make_hr(reply);
        return false;
    }
    return true;
}

bool UnitTestWithConnection::setTheDefaultCollectinState(MongoConnection& connection)
{
    std::vector<StringAndIntNoConstructor>               objects{{"DataString"s, 48},
                                                                 {"Another"s, 22},
                                                                 {"ThirdAndLast"s, 0xFF},
                                                                 {"ThisAndThat", 48},
                                                                 {"Bit The Dust", 22},
                                                                };

    std::size_t count;
    try
    {
        connection << send_CmdDB_Insert("test", "ConnectionTest", objects);
        connection >> get_CmdDB_InsertReply(count);
    }
    catch(MongoException const& e)
    {
        EXPECT_TRUE(false);
        std::cerr << e;
        return false;
    }

    EXPECT_EQ(5, count);

    if (count != 5)
    {
        return false;
    }
    return true;
}

