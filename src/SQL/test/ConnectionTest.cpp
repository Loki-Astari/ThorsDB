
#include "Connection.h"
#include "Statement.h"
#include "SQLConfig.h"

#include "gtest/gtest.h"
#include "test/MockSQL.h"


ThorsAnvil::SQL::Lib::ConnectionCreatorRegister<MockSQLConnection>   registerFakeMysql("mock");

TEST(ConnectionTest, Create)
{
    using ThorsAnvil::SQL::Connection;
    Connection     connection("mock://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
}
TEST(ConnectionTest, CreateDefaultPort)
{
    using ThorsAnvil::SQL::Connection;
    Connection     connection("mock://" THOR_TESTING_MYSQL_HOST , THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB);
}

TEST(ConnectionTest, BadSchema)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection("badschema://" THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB),
    std::runtime_error
    );
}

TEST(ConnectionTest, NoSchema)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection(THOR_TESTING_MYSQL_HOST ":69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB),
    std::logic_error
    );
}
TEST(ConnectionTest, BadHost)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection("mock://:69", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB),
    std::logic_error
    );
}
TEST(ConnectionTest, BadPort)
{
    using ThorsAnvil::SQL::Connection;
    ASSERT_THROW(
    Connection     connection("mock://" THOR_TESTING_MYSQL_HOST ":XY", THOR_TESTING_MYSQL_USER, THOR_TESTING_MYSQL_PASS, THOR_TESTING_MYSQL_DB),
    std::logic_error
    );
}
class MockConnectionProxy: public ThorsAnvil::SQL::Lib::ConnectionProxy
{
    public:
        virtual std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy> createStatementProxy(std::string const& statement) {return nullptr;}

        virtual int getSocketId() const  {return -1;}
        virtual void setYield(std::function<void()>&&, std::function<void()>&&) {}
};
TEST(ConnectionTest, YieldSetter)
{
    using ThorsAnvil::SQL::Connection;
    using ThorsAnvil::SQL::Lib::YieldSetter;

    MockConnectionProxy connection;
    YieldSetter     setYieldFunctions(connection, std::function<void()>([&connection](){}), std::function<void()>([&connection](){}));
}
