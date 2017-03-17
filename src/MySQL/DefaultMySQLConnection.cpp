#include "DefaultMySQLConnection.h"
#include "PrepareStatement.h"

using namespace ThorsAnvil::MySQL;

DefaultMySQLConnection::DefaultMySQLConnection(
                            std::string const& host, int port,
                            std::string const& username,
                            std::string const& password,
                            std::string const& database,
                            ThorsAnvil::SQL::Options const& options)
    : stream(host, port)
    , buffer(stream, true)
    , reader(buffer)
    , writer(buffer)
    , connection(username, password, database, options, reader, writer)
{}

std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy>
DefaultMySQLConnection::createStatementProxy(std::string const& statement)
{
    std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy>  result;
    result.reset(new PrepareStatement(connection, statement));
    return result;
}

ThorsAnvil::SQL::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlConnection("mysql");
