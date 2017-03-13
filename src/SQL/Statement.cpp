
#include "Statement.h"
#include "Connection.h"

using namespace ThorsAnvil::SQL;

        std::unique_ptr<StatementProxy>    statementProxy;
Statement::Statement(Connection& connect, std::string const& selectStatement, StatementType type)
    : statementProxy(connect.createStatementProxy(selectStatement, type))
{}

// -- StatementProxy

Cursor StatementProxy::execute()
{
    doExecute();
    return Cursor(*this);
}

// -- Cursor
Cursor::Cursor(StatementProxy& statementProxy)
    : statementProxy(statementProxy)
{}

Cursor::operator bool()
{
    return statementProxy.more();
}

#ifdef COVERAGE_SQL
#include "Statement.tpp"
template void ThorsAnvil::SQL::Cursor::activate<false, std::function<void (int, std::string const&, short, char, double)> >(std::function<void (int, std::string const&, short, char, double)>);
template void ThorsAnvil::SQL::Cursor::activate<true,  std::function<void (int, std::string const&, short, char, double)> >(std::function<void (int, std::string const&, short, char, double)>);
template void ThorsAnvil::SQL::BindArgs<int>::bindTo(ThorsAnvil::SQL::StatementProxy&) const;
#endif

