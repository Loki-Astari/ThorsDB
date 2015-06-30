
#ifndef THORSANVIL_MYSQL_DETAIL_RESP_PACKAGE_COLUMN_DEFINITION_H
#define THORSANVIL_MYSQL_DETAIL_RESP_PACKAGE_COLUMN_DEFINITION_H

#include "ConectReader.h"
#include <string>
#include <iostream>
#include <vector>
#include "ThorMySQL.h"

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {

struct RespPackageColumnDefinition
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

    friend std::ostream& operator<<(std::ostream& s, RespPackageColumnDefinition const& data) {
        s << "RespPackageColumnDefinition:"
          << " catalog: " << data.catalog
          << " schema: " << data.schema
          << " table: " << data.table
          << " orgTable: " << data.orgTable
          << " name: " << data.name
          << " orgName: " << data.orgName
          << " lengthOfFixedField: " << data.lengthOfFixedField
          << " charSet: " << data.charSet
          << " columnLength: " << data.columnLength
          << " type: " << data.type
          << " flags: " << data.flags
          << " decimal: " << data.decimal
          << " filler: (";
        for(auto const& dv: data.defaultValues) {
            s << dv << ", ";
        }
        s << ")\n";
        return s;
    }

    RespPackageColumnDefinition(ConectReader& reader, bool getDefaultValues = false)
    {
        unsigned long capabilities  = reader.getCapabilities();
        if (capabilities & CLIENT_PROTOCOL_41)
        {
            std::cerr << "CLIENT_PROTOCOL_41\n";
            catalog         = reader.lengthEncodedString();
            schema          = reader.lengthEncodedString();
            table           = reader.lengthEncodedString();
            orgTable        = reader.lengthEncodedString();
            name            = reader.lengthEncodedString();
            orgName         = reader.lengthEncodedString();
            std::cerr << "catalog: " << catalog << "  schema: " << schema << "  table: " << table << "  orgTable: " << orgTable << "  name: " << name << " orgName: " << orgName << "\n";

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
            std::cerr << "charSet: " << charSet << "  columnLength: " << columnLength << "  type: " << type << "  flags: " << flags << "  decimal: " << decimal << "  filler: " << filler << "\n";
            if (filler != 0) {
                throw std::runtime_error("Expected 0x00 for filler");
            }
        }
        else
        {
            std::cerr << "Normal\n";
            table           = reader.lengthEncodedString();
            name            = reader.lengthEncodedString();
            std::size_t len = reader.lengthEncodedInteger();
            std::cerr << "table: " << table << "  name: " << name << "  len: " << len << "\n";
            if (len != 0x03) {
                throw std::runtime_error("Expected 0x03: length of the column_length field [03]");
            }
            columnLength    = reader.fixedLengthInteger<3>();
            len             = reader.lengthEncodedInteger();
            std::cerr << "columnLength: " << columnLength << "  len: " << len << "\n";
            if (len != 0x01) {
                throw std::runtime_error("Expected 0x01: length of type field [01]");
            }
            type            = reader.fixedLengthInteger<1>();
            std::cerr << "type: " << type << "\n";

            if (capabilities & CLIENT_LONG_FLAG)
            {
                std::cerr << "LONG FLAG\n";
                len         = reader.lengthEncodedInteger();
                std::cerr << "len: " << len << "\n";
                if (len != 0x03) {
                    throw std::runtime_error("Expected 0x03: length of flags+decimals fields [03]");
                }
                flags           = reader.fixedLengthInteger<2>();
                decimal         = reader.fixedLengthInteger<1>();
                std::cerr << "flags: " << flags << "  decimal: " << decimal << "\n";
            }
            else
            {
                std::cerr << "NORMAL FLAG\n";
                len         = reader.lengthEncodedInteger();
                std::cerr << "len: " << len << "\n";
                if (len != 0x02) {
                    throw std::runtime_error("Expected 0x02: length of flags+decimals fields [02]");
                }
                flags           = reader.fixedLengthInteger<1>();
                decimal         = reader.fixedLengthInteger<1>();
                std::cerr << "flags: " << flags << "  decimal: " << decimal << "\n";
            }
        }
        if (getDefaultValues) {
            std::size_t len = reader.lengthEncodedInteger();
            for(std::size_t loop=0;loop < len; ++loop) {
                defaultValues.push_back(reader.lengthEncodedString());
                std::cerr << "     Def: " << defaultValues.size() << " : " << defaultValues.back() << "\n";
            }
        }
    }
};

        }
    }
}

#endif

