
#include "ThorSQL/Connection.h"
#include "ThorSQL/Statement.h"
#include "DefaultPostGConnection.h"
#include <gtest/gtest.h>

namespace TSQL = ThorsAnvil::DB::SQL;
namespace PSQL = ThorsAnvil::DB::Postgres;

ThorsAnvil::DB::SQL::Lib::ConnectionCreatorRegister<PSQL::DefaultPostGConnection>    postGTestConnection("postgres");


TEST(PostgresTest, BaseLine)
{
    TSQL::Connection    connection("postgres://localhost", "test", "testPassword", "test");
    TSQL::Statement     statement(connection, "SELECT * FROM People");
}
