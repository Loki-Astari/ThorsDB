
#include "Connection.h"
#include "ConectReader.h"
#include "ConectWriter.h"
#include "PackageBuffer.h"
#include "PostgresStream.h"
#include "gtest/gtest.h"

using ThorsAnvil::Postgres::Connection;
using ThorsAnvil::Postgres::ConectReader;
using ThorsAnvil::Postgres::ConectWriter;
using ThorsAnvil::Postgres::PackageBuffer;
using ThorsAnvil::Postgres::PostgresStream;

TEST(ConnectionTest, ConnectToDB)
{
    PostgresStream  stream("localhost");
    PackageBuffer   buffer(stream);
    ConectReader    reader(buffer);
    ConectWriter    writer(buffer);
    Connection      connection("test", "testPassword", "test", {}, reader, writer);
}
