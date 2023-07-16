#ifndef THORSANVIL_DB_MONGO_TEST_CONNECTED_ACTION_H
#define THORSANVIL_DB_MONGO_TEST_CONNECTED_ACTION_H

#include <gtest/gtest.h>
#include "test/OpTest.h"
#include "MongoConnection.h"

#include <memory>

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


class UnitTestWithConnection : public ::testing::Test
{
    public:
        UnitTestWithConnection();
        static void SetUpTestCase();
        static void TearDownTestCase();

        static ThorsAnvil::DB::Mongo::MongoConnection& getConnection();
        static bool checkTheNumberofObjectsIs(std::string const& message, ThorsAnvil::DB::Mongo::MongoConnection& connection, int expected);
        static bool zeroOutCollection(ThorsAnvil::DB::Mongo::MongoConnection& connection);
        static bool setTheDefaultCollectinState(ThorsAnvil::DB::Mongo::MongoConnection& connection);
    protected:
        static void setCollectionToBaseLine(ThorsAnvil::DB::Mongo::MongoConnection& connection);
    private:
        static std::unique_ptr<ThorsAnvil::DB::Mongo::MongoConnection> connection;
};

#endif
