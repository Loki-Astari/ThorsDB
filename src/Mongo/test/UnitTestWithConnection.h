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
        static  void SetUpTestSuite()           {}
        static  void TearDownTestSuite()        {}
        virtual void SetUp()    override;
        virtual void TearDown()  override;

        static ThorsAnvil::DB::Mongo::MongoConnection& getConnection();
        static bool checkTheNumberofObjectsIs(std::string const& message, ThorsAnvil::DB::Mongo::MongoConnection& connection, int expected);

    private:
        static bool zeroOutCollection(ThorsAnvil::DB::Mongo::MongoConnection& connection);
        static bool setTheDefaultCollectinState(ThorsAnvil::DB::Mongo::MongoConnection& connection);
        static void setCollectionToBaseLine(ThorsAnvil::DB::Mongo::MongoConnection& connection);
        static std::unique_ptr<ThorsAnvil::DB::Mongo::MongoConnection> connection;
};

#endif
