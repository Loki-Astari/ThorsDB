#ifndef THORS_ANVIL_SQL_PREPARE_STATEMENT_H
#define THORS_ANVIL_SQL_PREPARE_STATEMENT_H

#include "Statement.h"
#include "RespPackageResultSet.h"
#include "TypeReadWrite.h"
#include "PackageStream.h"
#include "ConectWriter.h"
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

    class BindBuffer
    {
        class BindStream: public MySQLStream
        {
            std::vector<char>& dst;
            public:
                BindStream(std::vector<char>& dst)
                    : MySQLStream(-1)
                    , dst(dst)
                {}
                virtual void read(char*, std::size_t) override {}
                virtual void write(char const* buffer, std::size_t len)  override
                {
                    dst.insert(dst.end(), buffer, buffer + len);
                }
        };
        std::vector<Detail::RespPackageColumnDefinition> const& columns;
        std::vector<char>                                       typeBuffer;
        std::vector<char>                                       valueBuffer;
        std::size_t                                             currentCol;
        BindStream                                              bindStream;
        ConectWriter                                            bindWriter;

        public:
            BindBuffer(std::vector<Detail::RespPackageColumnDefinition> const& col)
                : columns(col)
                , currentCol(0)
                , bindStream(valueBuffer)
                , bindWriter(bindStream)
            {}
            void bindToMySQL(ConectWriter& writer) const
            {
                // https://dev.mysql.com/doc/internals/en/com-stmt-execute.html#com-stmt-execute
                //  Called once to write information about all the bound parameters.
                if (columns.size() == 0)
                {
                    return;
                }
                std::vector<char>  null((columns.size() + 7) / 8);
                writer.writeRawData(&null[0], null.size());

                writer.writeFixedLengthInteger<1>(1);
                writer.writeRawData(&typeBuffer[0], typeBuffer.size());
                writer.writeRawData(&valueBuffer[0], valueBuffer.size());
            }
            void reset()
            {
                currentCol  = 0;
                typeBuffer.clear();
                valueBuffer.clear();
            }
            std::size_t countBoundParameters() const
            {
                return currentCol;
            }
            template<typename Src>
            void bindValue(Src const& value)
            {
                //  This is called once for each bound parameter.
                //  It build's up two chunks of data that are passed to the server.
                unsigned int type = Detail::writeParameterValue(bindWriter, value);
                typeBuffer.push_back(type);
                typeBuffer.push_back(std::is_unsigned<Src>::value ? '\x80' : '\x00');
                ++currentCol;
            }
    };
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
        bool                                            errorReading;
        public:
            ValidatorStream(std::vector<Detail::RespPackageColumnDefinition> const& colu);
            virtual void read(char* buffer, std::size_t len) override;
            bool  tooMany() const;
            bool  tooFew() const;
            void  reset() override;
    };

    Connection&                                         connection;
    std::unique_ptr<Detail::RespPackagePrepare>         prepareResp;
    int                                                 statementID;
    ValidatorStream                                     validatorStream;
    ConectReader                                        validatorReader;
    std::unique_ptr<Detail::RespPackagePrepareExecute>  prepareExec;
    std::unique_ptr<Detail::RespPackageResultSet>       nextLine;
    BindBuffer                                          bindBuffer;

    public:
        PrepareStatement(Connection& connection, std::string const& statement);
        ~PrepareStatement();

    private:
        virtual void abort()                                override;

        virtual void doExecute()                            override;
        virtual bool more()                                 override;

        virtual void   bind(char value)                     override    {bindBuffer.bindValue(value);}
        virtual void   bind(signed char value)              override    {bindBuffer.bindValue(value);}
        virtual void   bind(signed short value)             override    {bindBuffer.bindValue(value);}
        virtual void   bind(signed int value)               override    {bindBuffer.bindValue(value);}
        virtual void   bind(signed long value)              override    {bindBuffer.bindValue(value);}
        virtual void   bind(signed long long value)         override    {bindBuffer.bindValue(value);}
        virtual void   bind(unsigned char value)            override    {bindBuffer.bindValue(value);}
        virtual void   bind(unsigned short value)           override    {bindBuffer.bindValue(value);}
        virtual void   bind(unsigned int value)             override    {bindBuffer.bindValue(value);}
        virtual void   bind(unsigned long value)            override    {bindBuffer.bindValue(value);}
        virtual void   bind(unsigned long long value)       override    {bindBuffer.bindValue(value);}

        virtual void   bind(float value)                    override    {bindBuffer.bindValue(value);}
        virtual void   bind(double value)                   override    {bindBuffer.bindValue(value);}
        virtual void   bind(long double value)              override    {bindBuffer.bindValue(value);}

        virtual void   bind(std::string const& value)       override    {bindBuffer.bindValue(value);}
        virtual void   bind(std::vector<char> const& value) override    {bindBuffer.bindValue(value);}

        virtual void   bind(SQL::UnixTimeStamp const& value)override    {bindBuffer.bindValue(value);}

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
