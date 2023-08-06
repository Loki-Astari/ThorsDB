#ifndef THORSANVIL_DB_MYSQL_DETAIL_RESP_PACKAGE_RESULT_SET_H
#define THORSANVIL_DB_MYSQL_DETAIL_RESP_PACKAGE_RESULT_SET_H

#include "RespPackage.h"
#include "RespPackageColumnDefinition.h"
#include "TypeReadWrite.h"
#include "ThorsIOUtil/Utility.h"
#include "ThorsLogging/ThorsLogging.h"
#include <ostream>
#include <vector>
#include <string>
// #include <cstddef>   // for size_t (removed because it crashes clang 3.5 on travis
#include <stdexcept>

namespace ThorsAnvil::DB::MySQL
{

template<typename T> inline T readNullParameter()
{
    ThorsLogAndThrowLogical("ThorsAnvil::DB::MySQL",
                            "readNullParameter",
                            "Undefined for this type ", typeid(T).name());
}

template<> inline std::string readNullParameter<std::string>()                                 {return "";}
template<> inline int         readNullParameter<int>()                                         {return 0;}

class ConectReader;

class RespPackageResultSet: public RespPackage
{
    ConectReader&   reader;
    std::vector<RespPackageColumnDefinition> const& columns;
    std::string     nullMap;
    std::size_t     nextColumn;

    template<typename T>
    T readNextValue()
    {
        if (nextColumn >= columns.size())
        {
            /* Note: This should never happen as we already validate that
                     a user callback function has the exact parameters required
                     for the data provided by the server.

                     Because this should never happen it is a domain_error
            */
            ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::RespPackageResultSet",
                                     "readNextValue",
                                     "Trying to read more parameters than exist");
        }
        int currentColumn   = nextColumn;
        ++nextColumn;
        switch (columns[currentColumn].type)
        {
            case MYSQL_TYPE_STRING:         return readParameterValue<MYSQL_TYPE_STRING, T>(reader);
            case MYSQL_TYPE_VARCHAR:        return readParameterValue<MYSQL_TYPE_VARCHAR, T>(reader);
            case MYSQL_TYPE_VAR_STRING:     return readParameterValue<MYSQL_TYPE_VAR_STRING, T>(reader);
            case MYSQL_TYPE_LONG_BLOB:      return readParameterValue<MYSQL_TYPE_LONG_BLOB, T>(reader);
            case MYSQL_TYPE_MEDIUM_BLOB:    return readParameterValue<MYSQL_TYPE_MEDIUM_BLOB, T>(reader);
            case MYSQL_TYPE_BLOB:           return readParameterValue<MYSQL_TYPE_BLOB, T>(reader);
            case MYSQL_TYPE_TINY_BLOB:      return readParameterValue<MYSQL_TYPE_TINY_BLOB, T>(reader);
            case MYSQL_TYPE_BIT:            return readParameterValue<MYSQL_TYPE_BIT, T>(reader);
            case MYSQL_TYPE_DECIMAL:        return readParameterValue<MYSQL_TYPE_DECIMAL, T>(reader);
            case MYSQL_TYPE_NEWDECIMAL:     return readParameterValue<MYSQL_TYPE_NEWDECIMAL, T>(reader);
            case MYSQL_TYPE_LONGLONG:       return columns[currentColumn].flags & 0x20
                                                    ? readParameterValue<MYSQL_TYPE_LONGLONG_UNSIGNED, T>(reader)
                                                    : readParameterValue<MYSQL_TYPE_LONGLONG, T>(reader);
            case MYSQL_TYPE_LONG:           return columns[currentColumn].flags & 0x20
                                                    ? readParameterValue<MYSQL_TYPE_LONG_UNSIGNED, T>(reader)
                                                    : readParameterValue<MYSQL_TYPE_LONG, T>(reader);
            case MYSQL_TYPE_INT24:          return columns[currentColumn].flags & 0x20
                                                    ? readParameterValue<MYSQL_TYPE_INT24_UNSIGNED, T>(reader)
                                                    : readParameterValue<MYSQL_TYPE_INT24, T>(reader);
            case MYSQL_TYPE_SHORT:          return columns[currentColumn].flags & 0x20
                                                    ? readParameterValue<MYSQL_TYPE_SHORT_UNSIGNED, T>(reader)
                                                    : readParameterValue<MYSQL_TYPE_SHORT, T>(reader);
            case MYSQL_TYPE_YEAR:           return readParameterValue<MYSQL_TYPE_YEAR, T>(reader);
            case MYSQL_TYPE_TINY:           return columns[currentColumn].flags & 0x20
                                                    ? readParameterValue<MYSQL_TYPE_TINY_UNSIGNED, T>(reader)
                                                    : readParameterValue<MYSQL_TYPE_TINY, T>(reader);
            case MYSQL_TYPE_DOUBLE:         return readParameterValue<MYSQL_TYPE_DOUBLE, T>(reader);
            case MYSQL_TYPE_FLOAT:          return readParameterValue<MYSQL_TYPE_FLOAT, T>(reader);
            case MYSQL_TYPE_DATE:           return readParameterValue<MYSQL_TYPE_DATE, T>(reader);
            case MYSQL_TYPE_DATETIME:       return readParameterValue<MYSQL_TYPE_DATETIME, T>(reader);
            case MYSQL_TYPE_TIMESTAMP:      return readParameterValue<MYSQL_TYPE_TIMESTAMP, T>(reader);
            case MYSQL_TYPE_TIME:           return readParameterValue<MYSQL_TYPE_TIME, T>(reader);

            case MYSQL_TYPE_NULL:
            case MYSQL_TYPE_NEWDATE:
            case MYSQL_TYPE_TIMESTAMP2:
            case MYSQL_TYPE_DATETIME2:
            case MYSQL_TYPE_TIME2:
            case MYSQL_TYPE_ENUM:
            case MYSQL_TYPE_SET:
            case MYSQL_TYPE_GEOMETRY:
                ThorsLogAndThrowLogical("ThorsAnvil::DB::MySQL::RespPackageResultSet",
                                        "readNextValue",
                                        "This type is currently NOT unsupported.\n\n",
                                        "\tMySQL Type: ",  mapMySQLTypeToString(columns[currentColumn].type), "\n\n",
                                        "Send my some SQL and unit tests :-)");
            default:
                ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::RespPackageResultSet",
                                         "readNextValue",
                                         "Unimplemented Column Type");
        }
    }

    template<typename T>
    T getValue(T const& defaultValue)
    {
        int     index   = nextColumn / 8;
        int     bit     = nextColumn % 8;
        if (nullMap[index] & (1 << bit))
        {
            return defaultValue;
        }
        return readNextValue<T>();
    }
    public:
        RespPackageResultSet(int firstByte, ConectReader& reader, std::vector<RespPackageColumnDefinition> const& columns);
        virtual  std::ostream& print(std::ostream& s) const {return s;}

        void   retrieve(char& value)                        {value = getValue<char>('\0');}
        void   retrieve(signed char& value)                 {value = getValue<signed char>('\0');}
        void   retrieve(signed short& value)                {value = getValue<signed short>(0);}
        void   retrieve(signed int& value)                  {value = getValue<signed int>(0);}
        void   retrieve(signed long& value)                 {value = getValue<signed long>(0);}
        void   retrieve(signed long long& value)            {value = getValue<signed long long>(0);}
        void   retrieve(unsigned char& value)               {value = getValue<unsigned char>(0);}
        void   retrieve(unsigned short& value)              {value = getValue<unsigned short>(0);}
        void   retrieve(unsigned int& value)                {value = getValue<unsigned int>(0);}
        void   retrieve(unsigned long& value)               {value = getValue<unsigned long>(0);}
        void   retrieve(unsigned long long& value)          {value = getValue<unsigned long long>(0);}

        void   retrieve(float& value)                       {value = getValue<float>(0);}
        void   retrieve(double& value)                      {value = getValue<double>(0);}
        void   retrieve(long double& value)                 {value = getValue<long double>(0);}

        void   retrieve(std::string& value)                 {value = getValue<std::string>("");}

        void   retrieve(std::vector<char>& value)           {value = getValue<std::vector<char>>(std::vector<char>());};
        void   retrieve(Access::UnixTimeStamp& value)       {value = getValue<Access::UnixTimeStamp>(
                                                                        Access::UnixTimeStamp(static_cast<std::time_t>(0))
                                                                     );
                                                            }
};

}

#endif
