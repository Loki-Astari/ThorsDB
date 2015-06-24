
#ifndef THORS_ANVIL_MYSQL_STATEMENT_H
#define THORS_ANVIL_MYSQL_STATEMENT_H

#include "ThorSQL/Statement.h"

namespace ThorsAnvil
{
    namespace MySQL
    {

class Statement: public SQL::StatementProxy
{
    public:
        Statement(std::string const& statement);

    private:
};

    }
}

#endif

