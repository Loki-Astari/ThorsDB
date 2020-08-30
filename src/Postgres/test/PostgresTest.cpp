
#include "ThorsDB/Connection.h"
#include "ThorsDB/Statement.h"
#include "DefaultPostGConnection.h"
#include <gtest/gtest.h>

namespace TSQL = ThorsAnvil::DB::Access;
namespace PSQL = ThorsAnvil::DB::Postgres;

ThorsAnvil::DB::Access::Lib::ConnectionCreatorRegister<PSQL::DefaultPostGConnection>    postGTestConnection("postgres");


TEST(PostgresTest, BaseLine)
{
    TSQL::Connection    connection("postgres://localhost", "test", "testPassword", "test");
    TSQL::Statement     statement(connection, "SELECT * FROM People");
}
