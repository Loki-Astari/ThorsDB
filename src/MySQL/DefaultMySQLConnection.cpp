#include "DefaultMySQLConnection.h"
#include "PrepareStatement.h"

using namespace ThorsAnvil::DB::MySQL;

DefaultMySQLConnection::DefaultMySQLConnection(
                            std::string const& host, int port,
                            std::string const& username,
                            std::string const& password,
                            std::string const& database,
                            ThorsAnvil::DB::SQL::Options const& options)
    : stream(host, port)
    , buffer(stream, true)
    , reader(buffer)
    , writer(buffer)
    , connection(username, password, database, options, reader, writer)
{}

std::unique_ptr<ThorsAnvil::DB::SQL::Lib::StatementProxy>
DefaultMySQLConnection::createStatementProxy(std::string const& statement)
{
    std::unique_ptr<ThorsAnvil::DB::SQL::Lib::StatementProxy>  result;
    result.reset(new PrepareStatement(connection, statement));
    return result;
}

int DefaultMySQLConnection::getSocketId() const
{
    return stream.getSocketId();
}

void DefaultMySQLConnection::setYield(std::function<void()>&& yr, std::function<void()>&& yw)
{
    stream.setYield(std::move(yr), std::move(yw));
}

ThorsAnvil::DB::SQL::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlConnection("mysql");
