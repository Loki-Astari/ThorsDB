
#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "PackageBuffer.h"
#include "PostgresStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::DB::Postgres::Connection;
using ThorsAnvil::DB::Postgres::ConectReader;
using ThorsAnvil::DB::Postgres::ConectWriter;
using ThorsAnvil::DB::Postgres::PackageBuffer;
using ThorsAnvil::DB::Postgres::PostgresStream;

TEST(ConnectionTest, ConnectToDB)
{
#if THOR_DISABLE_POSTGRESS_TEST_SERVER_NOT_INSTALLED
    GTEST_SKIP() << "Postgress DB not installed";
#endif
    PostgresStream  stream("localhost");
    PackageBuffer   buffer(stream);
    ConectReader    reader(buffer);
    ConectWriter    writer(buffer);
    Connection      connection("test", "testPassword", "test", {}, reader, writer);
}
