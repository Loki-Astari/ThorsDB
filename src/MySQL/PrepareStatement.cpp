
#include "PrepareStatement.h"
#include "Connection.h"
#include "ConectReader.h"
#include "RespPackageEOF.h"
#include "ThorMySQL.h"
#include <stdexcept>


namespace ThorsAnvil { namespace SQL { namespace Detail {

}   }   }

using namespace ThorsAnvil::MySQL;

Detail::RequPackagePrepare::RequPackagePrepare(std::string const& statement)
    : RequPackage("RequPackagePrepare")
    , statement(statement)
{}

void Detail::RequPackagePrepare::build(ConectWriter& writer) const
{
    writer.writeFixedLengthInteger<1>(0x16);
    writer.writeVariableLengthString(statement);
}

Detail::RespPackagePrepare::RespPackagePrepare(ConectReader& reader)
    : RespPackage(reader)
{
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
        std::unique_ptr<RespPackage> mark = reader.getNextPackage(0xFE, [](ConectReader& reader){return new Detail::RespPackageEOF(reader);});
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
        std::unique_ptr<RespPackage> mark = reader.getNextPackage(0xFE, [](ConectReader& reader){return new Detail::RespPackageEOF(reader);});
        if (mark->isError()) {
            throw std::runtime_error(std::string("Expecting EOF markere afer Column info package: ") + mark->message());
        }
    }
}

Detail::ColumnDefinition::ColumnDefinition(ConectReader& reader, bool getDefaultValues)
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
                                    [](ConectReader& reader){return new Detail::RespPackagePrepare(reader);}
                              );
}

