
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "DefaultPostGConnection.h"
#include <gtest/gtest.h>

namespace TSQL = ThorsAnvil::SQL;
namespace PSQL = ThorsAnvil::Postgres;

ThorsAnvil::SQL::Lib::ConnectionCreatorRegister<PSQL::DefaultPostGConnection>    postGTestConnection("postgres");


TEST(PostgresTest, BaseLine)
{
    TSQL::Connection    connection("postgres://localhost", "test", "testPassword", "test");
    TSQL::Statement     statement(connection, "SELECT * FROM People");
}
