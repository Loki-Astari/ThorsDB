
#include "Connection.h"
#include "ThorSQL/Connection.h"
#include "PackageStream.h"
#include "PackageReader.h"

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
    SQL::Connection     connection("mysql://127.0.0.1", "root", "testPassword", options);
}

class DebugStream: public ThorsAnvil::MySQL::PackageStream
{
    public:
        virtual void        read(char* buffer, std::size_t len) override
        {
        }
        virtual bool        isEmpty()                           override
        {
            return true;
        }
        virtual std::string readRemainingData()                 override
        {
            return "";
        }
};
TEST(ConnectionTest, Create)
{
    using namespace ThorsAnvil;
    std::map<std::string, std::string>      options;

    DebugStream             stream;
    MySQL::PackageReader    reader(stream);
    MySQL::Connection       connection("root", "testPassword", options, reader);
}


