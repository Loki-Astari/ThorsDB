#include "Statement.h"
#include "Connection.h"

using namespace ThorsAnvil::DB::SQL;

std::unique_ptr<Lib::StatementProxy>    statementProxy;

Statement::Statement(Connection& connect, std::string const& selectStatement)
    : statementProxy(connect.createStatementProxy(selectStatement))
    , modifyDone(false)
{}

// -- StatementProxy

Detail::Cursor Lib::StatementProxy::execute()
{
    doExecute();
    return Detail::Cursor(*this);
}

// -- Cursor
Detail::Cursor::Cursor(Lib::StatementProxy& statementProxy)
    : statementProxy(statementProxy)
{}

Detail::Cursor::operator bool()
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

void Statement::execute()
{
    execute(Bind());
}


#ifdef COVERAGE_SQL
#include "Statement.tpp"
#include "MD5.h"
#include "test/SetGoodToTrue.h"

template
void ThorsAnvil::DB::SQL::Detail::Cursor::activate<false, std::function<void (int, std::string const&, short, char, double)>>
(std::function<void (int, std::string const&, short, char, double)>);

template void ThorsAnvil::DB::SQL::Detail::Cursor::activate<true,  std::function<void (int, std::string const&, short, char, double)>>
(std::function<void (int, std::string const&, short, char, double)>);

template void ThorsAnvil::DB::SQL::BindArgs<int>::bindTo(ThorsAnvil::DB::SQL::Lib::StatementProxy&) const;

template void ThorsAnvil::DB::SQL::BindArgs<>::bindTo(ThorsAnvil::DB::SQL::Lib::StatementProxy&) const;

template void ThorsAnvil::DB::SQL::Detail::Cursor::activate<false, std::function<void (int)> >(std::function<void (int)>);
template void ThorsAnvil::DB::SQL::Detail::Cursor::activate<true, std::function<void (int)> >(std::function<void (int)>);

template void ThorsAnvil::DB::SQL::Statement::execute<SetGoodToTrue>(SetGoodToTrue);
template void ThorsAnvil::DB::SQL::Statement::execute<SetGoodToTrue, int>(ThorsAnvil::DB::SQL::BindArgs<int> const&, SetGoodToTrue);
template void ThorsAnvil::DB::SQL::Statement::execute<CountLines, int>(ThorsAnvil::DB::SQL::BindArgs<int> const&, CountLines);
template void ThorsAnvil::DB::SQL::Statement::execute<int>(ThorsAnvil::DB::SQL::BindArgs<int> const&);

#endif