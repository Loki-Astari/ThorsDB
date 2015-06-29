
#include "PrepareStatement.h"
#include "Connection.h"
#include "ConectReader.h"
#include "RequPackage.h"
#include "RespPackage.h"
#include "RespPackageEOF.h"
#include "ThorMySQL.h"
#include <stdexcept>
#include <assert.h>


namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

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

    ColumnDefinition(ConectReader& reader, bool getDefaultValues = false)
    {
        unsigned long capabilities  = reader.getCapabilities();
        if (capabilities & CLIENT_PROTOCOL_41)
        {
            catalog         = reader.lengthEncodedString();
            schema          = reader.lengthEncodedString();
            table           = reader.lengthEncodedString();
            orgTable        = reader.lengthEncodedString();
            name            = reader.lengthEncodedString();
            orgName         = reader.lengthEncodedString();

            std::size_t len = reader.lengthEncodedInteger();
            if (len != 0x0c) {
                throw std::runtime_error("Expected 0x0c: length of fixed-length fields [0c]");
            }

            charSet         = reader.fixedLengthInteger<2>();
            columnLength    = reader.fixedLengthInteger<4>();
            type            = reader.fixedLengthInteger<1>();
            flags           = reader.fixedLengthInteger<2>();
            decimal         = reader.fixedLengthInteger<1>();
            filler          = reader.fixedLengthInteger<2>();
            if (filler != 0) {
                throw std::runtime_error("Expected 0x00 for filler");
            }
        }
        else
        {
            table           = reader.lengthEncodedString();
            name            = reader.lengthEncodedString();
            std::size_t len = reader.lengthEncodedInteger();
            if (len != 0x03) {
                throw std::runtime_error("Expected 0x03: length of the column_length field [03]");
            }
            columnLength    = reader.fixedLengthInteger<3>();
            len             = reader.lengthEncodedInteger();
            if (len != 0x01) {
                throw std::runtime_error("Expected 0x01: length of type field [01]");
            }
            type            = reader.fixedLengthInteger<1>();

            if (capabilities & CLIENT_LONG_FLAG)
            {
                len         = reader.lengthEncodedInteger();
                if (len != 0x03) {
                    throw std::runtime_error("Expected 0x03: length of flags+decimals fields [03]");
                }
                flags           = reader.fixedLengthInteger<2>();
                decimal         = reader.fixedLengthInteger<1>();
            }
            else
            {
                len         = reader.lengthEncodedInteger();
                if (len != 0x02) {
                    throw std::runtime_error("Expected 0x02: length of flags+decimals fields [02]");
                }
                flags           = reader.fixedLengthInteger<1>();
                decimal         = reader.fixedLengthInteger<1>();
            }
        }
        if (getDefaultValues) {
            std::size_t len = reader.lengthEncodedInteger();
            for(std::size_t loop=0;loop < len; ++loop) {
                defaultValues.push_back(reader.lengthEncodedString());
            }
        }
    }
};

class RequPackagePrepare: public RequPackage
{
    std::string const& statement;
    public:
        RequPackagePrepare(std::string const& statement)
            : RequPackage("RequPackagePrepare")
            , statement(statement)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}
        virtual  void build(ConectWriter& writer)       const override
        {
            writer.writeFixedLengthInteger<1>(0x16);
            writer.writeVariableLengthString(statement);
        }
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
        RespPackagePrepare(int firstByte, ConectReader& reader)
            : RespPackage(reader)
        {
            assert(firstByte == 0x00);

            // Not We have already read 1 byte (status OK)
            statementID     = reader.fixedLengthInteger<4>();
            numColumns      = reader.fixedLengthInteger<2>();
            numParams       = reader.fixedLengthInteger<2>();
                              reader.fixedLengthInteger<1>(); // reserved
            warningCount    = reader.fixedLengthInteger<2>();

            if (numParams > 0) {
                reader.reset();
                for(int loop = 0;loop < numParams; ++loop) {
                    paramInfo.emplace_back(reader);
                    reader.reset();
                }
                std::unique_ptr<RespPackage> mark = reader.getNextPackage(0xFE, [](int, ConectReader& reader){return new Detail::RespPackageEOF(reader);});
                if (mark->isError()) {
                    throw std::runtime_error(std::string("Expecting EOF markere afer Param info package: ") + mark->message());
                }
            }
            if (numColumns > 0) {
                reader.reset();
                for(int loop = 0;loop < numColumns; ++loop) {
                    columnInfo.emplace_back(reader);
                    reader.reset();
                }
                std::unique_ptr<RespPackage> mark = reader.getNextPackage(0xFE, [](int, ConectReader& reader){return new Detail::RespPackageEOF(reader);});
                if (mark->isError()) {
                    throw std::runtime_error(std::string("Expecting EOF markere afer Column info package: ") + mark->message());
                }
            }
        }

        virtual std::ostream& print(std::ostream& s)    const override  {return s;}
        int     getStatementID()                        const           {return statementID;}
};

        }
    }
}

using namespace ThorsAnvil::MySQL;

PrepareStatement::PrepareStatement(Connection& connectn, std::string const& statement)
    : Statement(statement)
    , connection(connectn)
{
    using Detail::RespPackagePrepare;
    using Detail::RequPackagePrepare;
    using PrepareResp = std::unique_ptr<RespPackagePrepare>;

    PrepareResp prepareResp = connection.sendMessage<RespPackagePrepare>(
                                    RequPackagePrepare(statement),
                                    Connection::Reset,
                                    0x00,
                                    [](int firstByte, ConectReader& reader){return new Detail::RespPackagePrepare(firstByte, reader);}
                              );
    statementID = prepareResp->getStatementID();
}

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {
class RequPackageExecute: public RequPackage
{
    int statementID;
    public:
        RequPackageExecute(int statementID)
            : RequPackage("")
            , statementID(statementID)
        {}
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}
        virtual  void build(ConectWriter& writer)       const override
        {
            writer.writeFixedLengthInteger<1>(0x17);
            writer.writeFixedLengthInteger<4>(statementID);
            /*
             *  0x00    CURSOR_TYPE_NO_CURSOR
             *  0x01    CURSOR_TYPE_READ_ONLY
             *  0x02    CURSOR_TYPE_FOR_UPDATE
             *  0x04    CURSOR_TYPE_SCROLLABLE
             */
            writer.writeFixedLengthInteger<1>(0x01);
            writer.writeFixedLengthInteger<4>(1);
        }
};

class RespPackageExecute: public RespPackage
{
    public:
        RespPackageExecute(int, ConectReader& reader)
            : RespPackage(reader)
        {
        }
        virtual  std::ostream& print(std::ostream& s)   const override {return s;}
};
        }
    }
}

void PrepareStatement::doExecute()
{
    connection.sendMessage<Detail::RespPackageExecute>(
                                    Detail::RequPackageExecute(statementID),
                                    Connection::Reset,
                                    0x05,
                                    [](int firstByte, ConectReader& reader){return new Detail::RespPackageExecute(firstByte, reader);}
                              );
}

bool PrepareStatement::more()
{
    return false;
}

#ifdef COVERAGE_MySQL
/*
 * This code is only compiled into the unit tests for code coverage purposes
 * It is not part of the live code.
 */
#include "Connection.tpp"

template std::unique_ptr<Detail::RespPackagePrepare> Connection::sendMessage<Detail::RespPackagePrepare, Detail::RequPackagePrepare>(Detail::RequPackagePrepare const&, Connection::PacketContinuation, int, std::function<RespPackage*(int, ConectReader&)>);

#endif

