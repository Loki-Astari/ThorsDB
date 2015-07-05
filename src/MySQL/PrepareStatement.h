
#ifndef THORS_ANVIL_SQL_PREPARE_STATEMENT_H
#define THORS_ANVIL_SQL_PREPARE_STATEMENT_H

#include "Statement.h"
#include "RespPackageResultSet.h"
#include "PackageStream.h"
#include <string>
#include <vector>

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {
            class RespPackagePrepare;
            class RespPackagePrepareExecute;
            class RespPackageResultSet;
            struct RespPackageColumnDefinition;
        }

class Connection;
class PrepareStatement: public Statement
{
    /*
     * I want to make sure that we use the same code for validating parameters
     * during the validation phase as during the execute phase.
     *
     * Therefore all calls to check parameters go through the class
     * RespPackageResultSet which knows how to convert from MySQL types to C++
     * types.
     *
     * During the validation phase (done before we execute a prepare statement)
     * We create a RespPackageResultSet object that uses the ValidatorStream to
     * read information from. This object is stored in `nextLine` below.
     *
     * This ValidatorStream basically constructs a stream of "default" values
     * based on the column definition returned by MySQL. Thus the code
     * is exercised using exactly the same path way as normal result object. The
     * difference is merly the stream is constructed from an internal string
     * rather being provided by the MySQL DB.
     *
     * A validation phase failure to convert will result in the same exception
     * as a runtime failure to convert. Thus we can most likely fail fast and thus
     * not require a call to the MySQL DB if the resulting data can not be converted.
     */
    class ValidatorStream: public MySQLStream
    {
        std::vector<Detail::RespPackageColumnDefinition> const& columns;
        std::string                                     validateInfo;
        std::size_t                                     position;
        public:
            ValidatorStream(std::vector<Detail::RespPackageColumnDefinition> const& colu);
            virtual void read(char* buffer, std::size_t len) override;
            bool  empty() const;
            void  reset();
    };

    Connection&                                         connection;
    std::unique_ptr<Detail::RespPackagePrepare>         prepareResp;
    int                                                 statementID;
    ValidatorStream                                     validatorStream;
    ConectReader                                        validatorReader;
    std::unique_ptr<Detail::RespPackagePrepareExecute>  prepareExec;
    std::unique_ptr<Detail::RespPackageResultSet>       nextLine;

    public:
        PrepareStatement(Connection& connection, std::string const& statement);
        ~PrepareStatement();

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

