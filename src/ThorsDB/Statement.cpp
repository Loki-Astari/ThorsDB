#include "Statement.h"
#include "Connection.h"

using namespace ThorsAnvil::DB::Access;

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
        ThorsLogAndThrowLogical("ThrosAnvil::Access::Statement", "rowsAffected", "Modification not attempted");
    }
    return statementProxy->rowsAffected();
}

long Statement::lastInsertID() const
{
    if (!modifyDone)
    {
        ThorsLogAndThrowLogical("ThrosAnvil::Access::Statement", "lastInsertID", "Modification not attempted");
    }
    return statementProxy->lastInsertID();
}

void Statement::execute()
{
    execute(Bind());
}
