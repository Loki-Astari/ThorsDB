#include "DefaultPostGConnection.h"
#include "Statement.h"
#include <string>

using namespace ThorsAnvil::DB::Postgres;

DefaultPostGConnection::DefaultPostGConnection(
                            std::string const& /*host*/, int /*port*/,
                            std::string const& /*username*/,
                            std::string const& /*password*/,
                            std::string const& /*database*/,
                            ThorsAnvil::DB::Access::Options const& /*options*/)
{}

std::unique_ptr<ThorsAnvil::DB::Access::Lib::StatementProxy>
DefaultPostGConnection::createStatementProxy(std::string const& statement)
{
    std::unique_ptr<ThorsAnvil::DB::Access::Lib::StatementProxy>  result;
    result.reset(new Statement(/*connection,*/ statement));
    return result;
}

ThorsAnvil::DB::Access::Lib::ConnectionCreatorRegister<DefaultPostGConnection>    postGConnection("postgres");
