
#include "Statement.h"
#include "Connection.h"

using namespace ThorsAnvil::SQL;

        std::unique_ptr<StatementProxy>    proxy;
Statement::Statement(Connection& connect, std::string const& selectStatement, StatementType type)
    : proxy(connect.createStatementProxy(selectStatement, type))
{}


