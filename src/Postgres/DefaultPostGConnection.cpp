#include "DefaultPostGConnection.h"
#include "Statement.h"
#include <string>

using namespace ThorsAnvil::Postgres;

DefaultPostGConnection::DefaultPostGConnection(
                            std::string const& /*host*/, int /*port*/,
                            std::string const& /*username*/,
                            std::string const& /*password*/,
                            std::string const& /*database*/,
                            ThorsAnvil::SQL::Options const& /*options*/)
{}

std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy>
DefaultPostGConnection::createStatementProxy(std::string const& statement)
{
    std::unique_ptr<ThorsAnvil::SQL::Lib::StatementProxy>  result;
    result.reset(new Statement(/*connection,*/ statement));
    return result;
}

ThorsAnvil::SQL::Lib::ConnectionCreatorRegister<DefaultPostGConnection>    postGConnection("postgres");
