#ifndef THORS_ANVIL_MYSQL_DETAIL_RESP_PACKAGE_COLUMN_DEFINITION_H
#define THORS_ANVIL_MYSQL_DETAIL_RESP_PACKAGE_COLUMN_DEFINITION_H

// MySQL documentation
// https://dev.mysql.com/doc/internals/en/com-query-response.html#packet-Protocol::ColumnDefinition

#include "ThorMySQL.h"
#include "TypeReadWrite.h"
#include <string>
#include <vector>
#include <ostream>
#include <iomanip>
#include <stdexcept>
//#include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis

namespace ThorsAnvil
{
    namespace MySQL
    {

class ConectReader;

struct RespPackageColumnDefinition
{
    std::string     catalog;
    std::string     schema;
    std::string     table;
    std::string     orgTable;
    std::string     name;
    std::string     orgName;
    std::size_t     lengthOfFixedField  = 0;
    int             charSet             = 0;
    int             columnLength        = 0;
    int             type                = 0;
    int             flags               = 0;
    int             decimal             = 0;
    int             filler              = 0;
    std::vector<std::string>    defaultValues;

    friend std::ostream& operator<<(std::ostream& s, RespPackageColumnDefinition const& data)
    {
        s << "RespPackageColumnDefinition:\n"
          << "\t" << std::setw(20) << std::left << "catalog: " << data.catalog << "\n"
          << "\t" << std::setw(20) << std::left << "schema: " << data.schema << "\n"
          << "\t" << std::setw(20) << std::left << "table: " << data.table << "\n"
          << "\t" << std::setw(20) << std::left << "orgTable: " << data.orgTable << "\n"
          << "\t" << std::setw(20) << std::left << "name: " << data.name << "\n"
          << "\t" << std::setw(20) << std::left << "orgName: " << data.orgName << "\n"
          << "\t" << std::setw(20) << std::left << "lengthOfFixedField: " << data.lengthOfFixedField << "\n"
          << "\t" << std::setw(20) << std::left << "charSet: " << data.charSet << "\n"
          << "\t" << std::setw(20) << std::left << "columnLength: " << data.columnLength << "\n"
          << "\t" << std::setw(20) << std::left << "type: " << data.type << "(" << mapMySQLTypeToString(data.type) << ")" << "\n"
          << "\t" << std::setw(20) << std::left << "flags: " << data.flags << "\n"
          << "\t" << std::setw(20) << std::left << "decimal: " << data.decimal << "\n"
          << "\t" << std::setw(20) << std::left << "filler: " << "(";
        for (auto const& dv: data.defaultValues)
        {
            s << dv << ", ";
        }
        s << ")\n";
        return s;
    }

    private:
    /* Allow the creation of fake columns.
     * But we don't want it to happen accidentally so you have to
     * explicitly ask for one via getFakeColumn().
     */
    RespPackageColumnDefinition(int type)
        : type(type)
    {}
    public:
    static RespPackageColumnDefinition getFakeColumn(int type)
    {
        return RespPackageColumnDefinition(type);
    }
    RespPackageColumnDefinition(ConectReader& reader, bool getDefaultValues = false)
    {
        // https://dev.mysql.com/doc/internals/en/com-query-response.html#packet-Protocol::ColumnDefinition
        unsigned long capabilities  = reader.getCapabilities();
        if (capabilities & CLIENT_PROTOCOL_41)
        {
            catalog         = reader.lengthEncodedString();
            schema          = reader.lengthEncodedString();
            table           = reader.lengthEncodedString();
            orgTable        = reader.lengthEncodedString();
            name            = reader.lengthEncodedString();
            orgName         = reader.lengthEncodedString();

            lengthOfFixedField = reader.lengthEncodedInteger();
            if (lengthOfFixedField != 0x0c)
            {
                throw std::domain_error(
                        errorMsg("ThorsAnvil::MySQL::RespPackageColumnDefinition::RespPackageColumnDefinition: ",
                                 "Expected 0x0c: length of fixed-length fields [0c]"
                      ));
            }

            charSet         = reader.fixedLengthInteger<2>();
            columnLength    = reader.fixedLengthInteger<4>();
            type            = reader.fixedLengthInteger<1>();
            flags           = reader.fixedLengthInteger<2>();
            decimal         = reader.fixedLengthInteger<1>();
            filler          = reader.fixedLengthInteger<2>();
            if (filler != 0)
            {
                throw std::domain_error(
                        errorMsg("ThorsAnvil::MySQL::RespPackageColumnDefinition::RespPackageColumnDefinition: ",
                                 "Expected 0x00 for filler"
                      ));
            }
        }
        else
        {
            table           = reader.lengthEncodedString();
            name            = reader.lengthEncodedString();
            size_t len = reader.lengthEncodedInteger();
            if (len != 0x03)
            {
                throw std::domain_error(
                        errorMsg("ThorsAnvil::MySQL::RespPackageColumnDefinition::RespPackageColumnDefinition: ",
                                 "Expected 0x03: length of the column_length field [03]"
                      ));
            }
            columnLength    = reader.fixedLengthInteger<3>();
            len             = reader.lengthEncodedInteger();
            if (len != 0x01)
            {
                throw std::domain_error(
                        errorMsg("ThorsAnvil::MySQL::RespPackageColumnDefinition::RespPackageColumnDefinition: ",
                                 "Expected 0x01: length of type field [01]"
                      ));
            }
            type            = reader.fixedLengthInteger<1>();

            if (capabilities & CLIENT_LONG_FLAG)
            {
                len         = reader.lengthEncodedInteger();
                if (len != 0x03)
                {
                    throw std::domain_error(
                            errorMsg("ThorsAnvil::MySQL::RespPackageColumnDefinition::RespPackageColumnDefinition: ",
                                     "Expected 0x03: length of flags+decimals fields [03]"
                          ));
                }
                flags           = reader.fixedLengthInteger<2>();
                decimal         = reader.fixedLengthInteger<1>();
            }
            else
            {
                len         = reader.lengthEncodedInteger();
                if (len != 0x02)
                {
                    throw std::domain_error(
                            errorMsg("ThorsAnvil::MySQL::RespPackageColumnDefinition::RespPackageColumnDefinition: ",
                                     "Expected 0x02: length of flags+decimals fields [02]"
                          ));
                }
                flags           = reader.fixedLengthInteger<1>();
                decimal         = reader.fixedLengthInteger<1>();
            }
        }
        if (getDefaultValues)
        {
            std::size_t len = reader.lengthEncodedInteger();
            for (std::size_t loop=0;loop < len; ++loop)
            {
                defaultValues.push_back(reader.lengthEncodedString());
            }
        }
    }
};

    }
}

#endif
