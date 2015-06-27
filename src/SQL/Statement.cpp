
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
inline Cursor::Cursor(StatementProxy& statementProxy)
    : statementProxy(statementProxy)
{}

inline Cursor::operator bool()
{
    return statementProxy.more();
}

#ifdef COVERAGE_TEST
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "Statement.tpp"
template void ThorsAnvil::SQL::BindArgs<int>::bindArgsTo<0ul>(ThorsAnvil::SQL::StatementProxy&, std::index_sequence<0ul> const&) const;

// template void ThorsAnvil::SQL::Cursor::activate<std::function<void (int)> >(std::function<void (int)>);
template void ThorsAnvil::SQL::BindArgs<int>::bindTo(ThorsAnvil::SQL::StatementProxy&) const;

template void ThorsAnvil::SQL::Cursor::activate<std::function<void (int, std::string const&, short, char, double)>>(std::function<void (int, std::string const&, short, char, double)>);

void forceInstanciation(ThorsAnvil::SQL::Cursor& cursor)
{
    // Force the instanciation of the normal function.
    // Cursor::operator bool()
    // Which means it is in this body and thus allows linking during coverage tests.
    if (static_cast<bool>(cursor)){}
}

#endif

