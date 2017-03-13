
#include "Connection.h"
#include "ThorSQL/Connection.h"
#include "ConectReader.h"
#include "PackageBuffer.h"
#include "PackageStream.h"

#include "gtest/gtest.h"


/*
 * Tests assume mysql is already up and running.
 * Default port 3306
 * Has a user called root with password testPassword
 */
TEST(ConnectionTest, CreateMySQLOnGeneric)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;
    SQL::Connection     connection("mysql://127.0.0.1", "root", "testPassword", "test", options);
}

TEST(ConnectionTest, Create)
{
    using namespace ThorsAnvil;
    using Buffer=MySQL::PackageBufferMySQLDebugBuffer<MySQL::MySQLStream>;
    std::map<std::string, std::string>      options;

    MySQL::MySQLStream      stream("127.0.0.1", 0);
    Buffer                  buffer(stream, true);
    MySQL::ConectReader     reader(buffer);
    MySQL::ConectWriter     writer(buffer);
    MySQL::Connection       connection("root", "testPassword", "test", options, reader, writer);
}


