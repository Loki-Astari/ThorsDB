
#ifndef THORS_ANVIL_SQL_PREPARE_STATEMENT_H
#define THORS_ANVIL_SQL_PREPARE_STATEMENT_H

#include "Statement.h"
#include "RespPackageResultSet.h"
#include <string>

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {
            class RespPackagePrepare;
            class RespPackageExecute;
            class RespPackageResultSet;
        }

class Connection;
class PrepareStatement: public Statement
{
    Connection&     connection;
    int             statementID;

    std::unique_ptr<Detail::RespPackagePrepare>     prepareResp;
    std::unique_ptr<Detail::RespPackageExecute>     prepareExec;
    std::unique_ptr<Detail::RespPackageResultSet>   nextLine;

    public:
        PrepareStatement(Connection& connection, std::string const& statement);

        virtual void doExecute()                            override;
        virtual bool more()                                 override;

        virtual void   retrieve(char& value)                override    {nextLine->retrieve(value);}
        virtual void   retrieve(signed char& value)         override    {nextLine->retrieve(value);}
        virtual void   retrieve(signed short& value)        override    {nextLine->retrieve(value);}
        virtual void   retrieve(signed int& value)          override    {nextLine->retrieve(value);}
        virtual void   retrieve(signed long& value)         override    {nextLine->retrieve(value);}
        virtual void   retrieve(signed long long& value)    override    {nextLine->retrieve(value);}
        virtual void   retrieve(unsigned char& value)       override    {nextLine->retrieve(value);}
        virtual void   retrieve(unsigned short& value)      override    {nextLine->retrieve(value);}
        virtual void   retrieve(unsigned int& value)        override    {nextLine->retrieve(value);}
        virtual void   retrieve(unsigned long& value)       override    {nextLine->retrieve(value);}
        virtual void   retrieve(unsigned long long& value)  override    {nextLine->retrieve(value);}

        virtual void   retrieve(float& value)               override    {nextLine->retrieve(value);}
        virtual void   retrieve(double& value)              override    {nextLine->retrieve(value);}
        virtual void   retrieve(long double& value)         override    {nextLine->retrieve(value);}

        virtual void   retrieve(std::string& value)         override    {nextLine->retrieve(value);}
        virtual void   retrieve(std::vector<char>& value)   override    {nextLine->retrieve(value);}

        virtual void   retrieve(SQL::UnixTimeStamp& value)  override    {nextLine->retrieve(value);}
};

    }
}

#endif

