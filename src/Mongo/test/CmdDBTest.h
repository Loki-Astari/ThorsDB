#ifndef THORSANVIL_CMD_DB_TEST_H
#define THORSANVIL_CMD_DB_TEST_H

#include "UnitTestWithConnection.h"

class CmdDBTest: public UnitTestWithConnection
{
    public:
        CmdDBTest()
        {
            setCollectionToBaseLine(getConnection());
        };
};

#endif
