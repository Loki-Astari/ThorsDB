
#ifndef THORS_ANVIL_SQL_STATEMENT_H
#define THORS_ANVIL_SQL_STATEMENT_H

#include "SQLUtil.h"
#include <memory>

namespace ThorsAnvil
{
    namespace SQL
    {

class StatementProxy
{
    public:
        virtual ~StatementProxy()  = 0;
};
inline StatementProxy::~StatementProxy() {}

class Connection;
class Statement
{
    private:
        std::unique_ptr<StatementProxy>    proxy;
    public:
        Statement(Connection& connect, std::string const& selectStatement, StatementType = ThorsAnvil::SQL::Prepare);
};

    }
}

#endif


