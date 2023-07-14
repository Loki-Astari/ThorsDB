
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
    GTEST_SKIP() << "DB not set up on actions";
    PostgresStream  stream("localhost");
    PackageBuffer   buffer(stream);
    ConectReader    reader(buffer);
    ConectWriter    writer(buffer);
    Connection      connection("test", "testPassword", "test", {}, reader, writer);
}
