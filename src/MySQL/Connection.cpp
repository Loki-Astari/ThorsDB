
#include "Connection.h"

using namespace ThorsAnvil::MySQL;

ThorsAnvil::SQL::ConnectionCreatorRegister<ThorsAnvil::MySQL::Connection>    mysqlConnection("mysql");

Connection::Connection(
                    std::string const& /*host*/, int /*port*/,
                    std::string const& /*username*/,
                    std::string const& /*password*/,
                    std::map<std::string, std::string> const& /*options*/)
{
}

