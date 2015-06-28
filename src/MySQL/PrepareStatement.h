
#ifndef THORS_ANVIL_SQL_PREPARE_STATEMENT_H
#define THORS_ANVIL_SQL_PREPARE_STATEMENT_H

#include "Statement.h"
#include <string>

namespace ThorsAnvil
{
    namespace MySQL
    {

class Connection;
class PrepareStatement: public Statement
{
    Connection&     connection;
    int             statementID;
    public:
        PrepareStatement(Connection& connection, std::string const& statement);

        virtual void doExecute()                            override;
        virtual bool more()                                 override;

};

    }
}

#endif

