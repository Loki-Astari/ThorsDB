#include "Statement.h"
#include "Connection.h"

using namespace ThorsAnvil::SQL;

        std::unique_ptr<StatementProxy>    statementProxy;
Statement::Statement(Connection& connect, std::string const& selectStatement, StatementType type)
    : statementProxy(connect.createStatementProxy(selectStatement, type))
    , modifyDone(false)
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

long Statement::rowsAffected() const
{
    if (!modifyDone)
    {
        throw std::logic_error("ThrosAnvil::SQL::Statement::rowsAffected: Modification not attempted");
    }
    return statementProxy->rowsAffected();
}

long Statement::lastInsertID() const
{
    if (!modifyDone)
    {
        throw std::logic_error("ThrosAnvil::SQL::Statement::rowsAffected: Modification not attempted");
    }
    return statementProxy->lastInsertID();
}

#ifdef COVERAGE_SQL
#include "Statement.tpp"
template
void ThorsAnvil::SQL::Cursor::activate<false, std::function<void (int, std::string const&, short, char, double)>>
(std::function<void (int, std::string const&, short, char, double)>);

template void ThorsAnvil::SQL::Cursor::activate<true,  std::function<void (int, std::string const&, short, char, double)>>
(std::function<void (int, std::string const&, short, char, double)>);

template void ThorsAnvil::SQL::BindArgs<int>::bindTo(ThorsAnvil::SQL::StatementProxy&) const;

template void ThorsAnvil::SQL::BindArgs<>::bindTo(ThorsAnvil::SQL::StatementProxy&) const;

template void ThorsAnvil::SQL::Cursor::activate<false, std::__1::function<void (int)> >(std::__1::function<void (int)>);
template void ThorsAnvil::SQL::Cursor::activate<true, std::__1::function<void (int)> >(std::__1::function<void (int)>);
#endif
