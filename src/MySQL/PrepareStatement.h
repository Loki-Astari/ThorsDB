
#ifndef THORS_ANVIL_SQL_PREPARE_STATEMENT_H
#define THORS_ANVIL_SQL_PREPARE_STATEMENT_H

#include "Statement.h"
#include "RequPackage.h"
#include "RespPackage.h"
#include <string>

namespace ThorsAnvil
{
    namespace MySQL
    {

class ConectReader;
class Connection;

        namespace Detail
        {

class RequPackagePrepare: public RequPackage
{
    std::string const& statement;
    public:
        RequPackagePrepare(std::string const& statement);
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}
        virtual  void build(ConectWriter& writer)       const override;
};

struct ColumnDefinition
{
    std::string     catalog;
    std::string     schema;
    std::string     table;
    std::string     orgTable;
    std::string     name;
    std::string     orgName;
    std::size_t     lengthOfFixedField;
    int             charSet;
    int             columnLength;
    int             type;
    int             flags;
    int             decimal;
    int             filler;
    std::vector<std::string>    defaultValues;

    ColumnDefinition(ConectReader& reader, bool getDefaultValues = false);
};

class RespPackagePrepare: public RespPackage
{
    int     statementID;
    int     numColumns;
    int     numParams;
    int     warningCount;
    std::vector<ColumnDefinition>   paramInfo;
    std::vector<ColumnDefinition>   columnInfo;
    public:
        RespPackagePrepare(ConectReader& reader);
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}
};
        }



class PrepareStatement: public Statement
{
    Connection&     connection;
    public:
        PrepareStatement(Connection& connection, std::string const& statement);
};

    }
}

#endif

